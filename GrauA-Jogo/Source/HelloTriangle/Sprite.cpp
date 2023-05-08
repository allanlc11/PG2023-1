#include "Sprite.h"
#include <GLFW/glfw3.h>

//Gravidade usada para calcular o pulo
const float gravity = -10.0f;

void Sprite::initialize(int texID, int imgWidth, int imgHeight, float parallaxFactor, int nAnimations, int nFrames)
{
	this->texID = texID;
	this->imgWidth = imgWidth;
	this->imgHeight = imgHeight;
	this->nAnimations = nAnimations;
	this->nFrames = nFrames;
	this->parallaxFactor = parallaxFactor;

	speed.x = 5.0;
	//Velocidade no eixo y n�o � usada para o pulo
	speed.y = 5.0;
	//Usado para trocar a anima��o de pulo
	airborne = false;

	dx = 1.0 / float(nFrames);
	dy = 1.0 / float(nAnimations);

	GLfloat vertices[] = {
		//x   y     z    r    g    b     s     t
		//T1
		-0.5,  0.5, 0.0, 0.0, 0.0, 0.0, 0.0, dy, //v0
		-0.5, -0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, //v1
		 0.5,  0.5, 0.0, 0.0, 0.0, 0.0, dx, dy, //v2
		 //T2
		-0.5, -0.5, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, //v1
		 0.5,  0.5, 0.0, 0.0, 0.0, 0.0, dx, dy, //v2
		 0.5, -0.5, 0.0, 0.0, 0.0, 0.0, dx, 0.0  //v3
	};

	GLuint VBO;

	//Gera��o do identificador do VBO
	glGenBuffers(1, &VBO);
	//Faz a conex�o (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Gera��o do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de v�rtices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localiza��o no shader * (a localiza��o dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se est� normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 

	//Atributo posi��o
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo cor
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//Atributo coordenada de textura
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// Observe que isso � permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de v�rtice 
	// atualmente vinculado - para que depois possamos desvincular com seguran�a
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (� uma boa pr�tica desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);
}

void Sprite::setShader(Shader* shader)
{
	this->shader = shader;
	shader->use();
}


void Sprite::draw()
{
	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_2D, texID);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

//Sobrecarga do m�todo update que ajusta a posi��o do sprite de acordo com o fator de parallax.
//Os sprites do fundo recebem um fator <1. Os no n�vel do personagem e do ch�o recebem 1. Os da frente, perto da "c�mera", receberiam >1 se houvesse algum.
void Sprite::update(float deltaX)
{
	position.x += deltaX * parallaxFactor;
	update();
}

void Sprite::update()
{
	glm::mat4 model = glm::mat4(1); //matriz identidade
	model = glm::translate(model, position);
	model = glm::scale(model, dimensions);
	int modelLoc = glGetUniformLocation(shader->ID, "model");
	glUniformMatrix4fv(modelLoc, 1, FALSE, glm::value_ptr(model));

	//Utilizado para limitar o frame rate das anima��es
	double currentTime = glfwGetTime();
	double deltaTime = currentTime - lastUpdateTime;

	if (deltaTime >= 1.0f / animationSpeed) {
		iFrame = (iFrame + 1) % nFrames;
		lastUpdateTime = currentTime;
	}

	//Se o personagem estivar no ar, calculamos o arco do pulo de acordo com a gravidade e o tempo.
	if (airborne) {
		jumpSpeed += gravity * deltaTime;
		position.y += jumpSpeed * deltaTime;

		//Se o personagem toca no ch�o (ou passa dele) p�e no ch�o, retira a propriedade a�rea, zera a velocidade de pulo
		//e atualiza a anima��o para a do ch�o correspondente � dire��o em que ele est� virado
		if (position.y <= 110) {
			position.y = 110;
			jumpSpeed = 0.0f;
			airborne = false;
			iAnimation = direction + 6;
		}

		//Troca o quadro da anima��o de acordo atrav�s do offset da folha de sprites
		float offsetx = iFrame * dx;
		float offsety;

		//Se a velocidade do pulo � positiva, p�e o personagem na posi��o de pulo de acordo com a dire��o em que ele est� virado.
		if (jumpSpeed > 0) iAnimation = direction + 2;
		//Se a velocidade do pulo � negativa, p�e o personagem na posi��o de queda de acordo com a dire��o
		else if (jumpSpeed < 0) iAnimation = direction + 4;

		//Troca a anima��o de acordo atrav�s do offset da folha de sprites
		offsety = iAnimation * dy;

		shader->setVec2("offsets", offsetx, offsety);
	}
	//Caso o personagem n�o estiver no ar, troca o quadro de anima��o (para os sprites com mais de 1 quadro) e a anima��o de acordo com o offset (para os sprites com ais de 1 quadro)
	else {
		float offsetx = 1, offsety = 1;
		if(nFrames >1)	offsetx = iFrame * dx;
		if (nAnimations > 1) offsety = iAnimation * dy;

		shader->setVec2("offsets", offsetx, offsety);
	}

}

void Sprite::getAABB(glm::vec2& min, glm::vec2& max)
{
	min.x = position.x - dimensions.x / 2.0;
	max.x = position.x + dimensions.x / 2.0;

	min.y = position.y - dimensions.y / 2.0;
	max.y = position.y + dimensions.y / 2.0;

} 