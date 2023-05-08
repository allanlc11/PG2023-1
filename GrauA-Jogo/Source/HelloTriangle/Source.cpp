#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


//STB IMAGE
#include "stb_image.h"

//Classe shader
#include "Shader.h"

//Classe sprite
#include "Sprite.h"


// Protótipo da função de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Protótipos das funções
int setupSprite();
GLuint generateTexture(string filePath, int& width, int& height);
bool testCollision(Sprite spr1, Sprite spr2);

// Dimensões da janela (pode ser alterado em tempo de execução)
const GLuint WIDTH = 800, HEIGHT = 600;

int moedasFaltando = 3;
int vidas = 3;
Sprite character;

// Função MAIN
int main()
{
	// Inicialização da GLFW
	glfwInit();

	// Criação da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "It's a me, Mario!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da função de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d funções da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informações de versão
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;


	// Compilando e buildando o programa de shader
	Shader shader("HelloTriangle.vs", "HelloTriangle.fs");

	int bgwidth, bgheight;
	int fgwidth, fgheight;
	int charwidth, charheight;
	int goombawidth, goombaheight;
	int coinwidth, coinheight;

	GLuint texID = generateTexture("../../textures/fundo.png", bgwidth, bgheight);
	GLuint texID1 = generateTexture("../../textures/frente.png", fgwidth, fgheight);
	GLuint texID2 = generateTexture("../../textures/mario.png", charwidth, charheight);
	GLuint texID3 = generateTexture("../../textures/goomba.png", goombawidth, goombaheight);
	GLuint texID4 = generateTexture("../../textures/moeda.png", coinwidth, coinheight);

	character.initialize(texID2, charwidth, charheight, 1.0f, 8, 3);
	character.setShader(&shader);
	character.setPosition(glm::vec3(100, 110, 0));
	character.setScale(glm::vec3(charwidth*2, charheight*2, 1));


	Sprite background;
	background.initialize(texID, bgwidth, bgheight, 0.5f);
	background.setShader(&shader);
	background.setPosition(glm::vec3(400, 300, 0));
	background.setScale(glm::vec3(bgwidth*1.6, bgheight*1.6, 1));

	Sprite foreground;
	foreground.initialize(texID1, fgwidth, fgheight, 1.0f);
	foreground.setShader(&shader);
	foreground.setPosition(glm::vec3(fgwidth/2+665, 300, 0));
	foreground.setScale(glm::vec3(fgwidth*1.3, fgheight*1.3, 1));

	//Aqui são configuradas as posições das moedas e goombas. Para evitar repetir o código, as moedas e goombas foram postos em arrays.
	//Os goombas e as moedas também são marcadas para serem "removidas" quando o personagem colidir com os sprites mais tarde.
	Sprite goombas[3];
	Sprite moedas[3];
	int goombaXPositions[3] = {250, 500, 750};
	int coinXPositions[3] = {100, 550, 800};
	bool pegouMoeda[3] = { false, false, false };
	bool bateuNoGoomba[3] = { false, false, false };	

	for (int i = 0; i < 3; i++) {
		goombas[i].initialize(texID3, goombawidth, goombaheight, 1.0f, 1, 2);
		goombas[i].setShader(&shader);
		goombas[i].setPosition(glm::vec3(goombaXPositions[i], 100, 0));
		goombas[i].setScale(glm::vec3(goombawidth * 1.5, goombaheight * 1.5, 1));

		moedas[i].initialize(texID4, coinwidth, coinheight, 1.0f, 1, 4);
		moedas[i].setShader(&shader);
		moedas[i].setPosition(glm::vec3(coinXPositions[i], 170, 0));
		moedas[i].setScale(glm::vec3(coinwidth * 2, coinheight * 2, 1));
	}
	
	glUseProgram(shader.ID);


	glm::mat4 projection = glm::ortho(0.0, 800.0, 0.0, 600.0, -1.0, 1.0);
	// Definindo as dimensões da viewport com as mesmas dimensões da janela da aplicação
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);

	GLint projLoc = glGetUniformLocation(shader.ID, "projection");
	glUniformMatrix4fv(projLoc, 1, FALSE, glm::value_ptr(projection));


	//Ativar o buffer de textura
	glActiveTexture(GL_TEXTURE0);

	//Habilitar teste de profundidade
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_ALWAYS);

	//Habilitar a transparência
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Valores usados para o scrolling
	float offsetX = 0;
	float ultimaPosicaoX = character.getPosition().x;
	double lastUpdateTime = glfwGetTime();

	// Loop da aplicação - "game loop"
	while (!glfwWindowShouldClose(window))
	{

		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as funções de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Valor usado para o scrolling
		offsetX = ultimaPosicaoX - character.getPosition().x;

		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastUpdateTime;
		//Move a projeção de acordo com o movimento do personagem, determinado na iteração anterior do loop com o offsetX
		projection = projection * glm::translate(glm::mat4(1.0f), glm::vec3(offsetX, 0.0f, 0.0f));;
		GLint projLoc = glGetUniformLocation(shader.ID, "projection");
		glUniformMatrix4fv(projLoc, 1, FALSE, glm::value_ptr(projection));

		//Move o fundo e o cenário (que, no sprite, vai usar o valor de parallax para determinar o quanto vai ser movido)
		background.update(offsetX);
		background.draw();

		foreground.update(offsetX);
		foreground.draw();

		//Itera sobre os arrays de moedas e goombas e desenha eles na tela (com o scrolling para que eles fiquem "parados" em relação ao chão) caso eles não tenham colidido com o personagem
		for (int i = 0; i < 3; ++i) {
			if (!bateuNoGoomba[i]) {
				goombas[i].update(offsetX);
				goombas[i].draw();
			}
			if (!pegouMoeda[i]) {
				moedas[i].update(offsetX);
				moedas[i].draw();
			}
		}

		character.update();
		character.draw();

		//marca posição do personagem para determinarmos o quanto ele se mexeu desde a última iteração do loop.
		ultimaPosicaoX = character.getPosition().x;

		//Testa colisão das moedas e goombas iterando sobre os arrays. Caso o personagem colidir, "remove" eles do jogo
		for (int i = 0; i < 3; i++) {
			if (testCollision(character, moedas[i]) && !pegouMoeda[i]) {
				pegouMoeda[i] = true;
				moedasFaltando--;
			}
			if (testCollision(character, goombas[i]) && !bateuNoGoomba[i]) {
				bateuNoGoomba[i] = true;
				vidas--;
			}
		}

		glBindVertexArray(0); //Desconectando o buffer de geometria

		// Troca os buffers da tela
		glfwSwapBuffers(window);

		//Fim de jogo
		//Por motivos de simplicidade, o jogo acaba quando o personagem pegar todas as moedas ou colidir com todos goombas.
		//Não foram implementadas mensagens positivas ou negativas de fim de jogo
		if (moedasFaltando == 0 || vidas == 0)
			glfwSetWindowShouldClose(window, GL_TRUE);
	}
	// Finaliza a execução da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_D || key == GLFW_KEY_RIGHT)
	{
		character.moveRight();
	}
	if (key == GLFW_KEY_A || key == GLFW_KEY_LEFT)
	{
		character.moveLeft();
	}
	if (key == GLFW_KEY_W || key == GLFW_KEY_UP)
	{
		character.jump();
	}

	//Retirei está funcionalidade por não ter animação ou desejar ter qualquer movimento associado.
	//Caso habilitado, permite que o personagem caia mais rápido e, embora permita que ele passe do chão, o pulo atualiza a posição para o chão no início.
	//if (key == GLFW_KEY_S || key == GLFW_KEY_DOWN)
	//{	
	//	character.moveDown();
	//}
	//Caso o personagem não estiver se mexendo, chama a função que confere se ele está no ar e põe ele na animação parada
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_A) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_S) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_D) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_UP) == GLFW_RELEASE &&
		glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_RELEASE) {
		character.stop();
	}
}

int setupSprite()
{
	GLfloat vertices[] = {
		//x   y     z    r    g    b     s     t
		//T1
		-0.5,  0.5, 0.0, 1.0, 0.0, 0.0, 0.0, 1.0, //v0
		-0.5, -0.5, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, //v1
		 0.5,  0.5, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, //v2
		 //T2
		-0.5, -0.5, 0.0, 0.0, 1.0, 0.0, 0.0, 0.0, //v1
		 0.5,  0.5, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, //v2
		 0.5, -0.5, 0.0, 0.0, 1.0, 0.0, 1.0, 0.0  //v3
	};

	GLuint VBO, VAO;

	//Geração do identificador do VBO
	glGenBuffers(1, &VBO);
	//Faz a conexão (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Geração do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);
	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de vértices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localização no shader * (a localização dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se está normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 

	//Atributo posição
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo cor
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//Atributo coordenada de textura
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// Observe que isso é permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de vértice 
	// atualmente vinculado - para que depois possamos desvincular com segurança
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (é uma boa prática desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}


GLuint generateTexture(string filePath, int& width, int& height)
{
	GLuint texID;
	// Gera o identificador da textura na memória 
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);

	//Definindo o método de wrapping e de filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	//Carregando a imagen da textura
	int nrChannels;
	unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

	//Manda para OpenGL armazenar a textura e gerar o mipmap
	if (data)
	{
		if (nrChannels == 3) //jpg, bmp
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else //png
		{
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		}
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}

	stbi_image_free(data);
	glBindTexture(GL_TEXTURE_2D, 0);

	return texID;
}

//Confere se 2 sprites colidem
bool testCollision(Sprite spr1, Sprite spr2)
{
	glm::vec2 min1, min2, max1, max2;
	spr1.getAABB(min1, max1);
	spr2.getAABB(min2, max2);

	if (min1.x < max2.x && min2.x < max1.x) {
		if (min1.y < max2.y && min2.y < max1.y) {
			return true;
		}
	}

	return false;
}