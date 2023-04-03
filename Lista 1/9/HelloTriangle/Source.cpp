#include <iostream>
#include <string>
#include <assert.h>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

//Classe shader
#include "Shader.h"

const int NLADOS = 50;


// Prot�tipo da fun��o de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

// Prot�tipos das fun��es
int setupGeometry();
int setupCircle(int secondWheel);

// Dimens�es da janela (pode ser alterado em tempo de execu��o)
const GLuint WIDTH = 800, HEIGHT = 600;


// Fun��o MAIN
int main()
{
	// Inicializa��o da GLFW
	glfwInit();

	// Cria��o da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Exerc�cio 8 - implementa��o", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da fun��o de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);

	// GLAD: carrega todos os ponteiros d fun��es da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informa��es de vers�o
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Definindo as dimens�es da viewport com as mesmas dimens�es da janela da aplica��o
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);


	// Compilando e buildando o programa de shader
	Shader shader("HelloTriangle.vs", "HelloTriangle.fs");

	// Corpo
	GLuint VAO_CORPO = setupGeometry();
	//Rodas
	GLuint VAO_RODA_ESQUERDA = setupCircle(0);
	GLuint VAO_RODA_DIREITA = setupCircle(1);
	
	
	glUseProgram(shader.ID);

	// Loop da aplica��o - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as fun��es de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		//glClearColor(0.0f, 0.59f, 0.22f, 1.0f); //cor de fundo
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glPointSize(20);
		glLineWidth(10);


		glBindVertexArray(VAO_RODA_ESQUERDA);
		glDrawArrays(GL_TRIANGLE_FAN, 0, NLADOS);
		glBindVertexArray(0);

		glBindVertexArray(VAO_RODA_DIREITA);
		glDrawArrays(GL_TRIANGLE_FAN, 0, NLADOS);
		glBindVertexArray(0);


		glBindVertexArray(VAO_CORPO);
		glDrawArrays(GL_TRIANGLE_FAN, 0, 8);
		glBindVertexArray(0);

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO_RODA_ESQUERDA);
	glDeleteVertexArrays(1, &VAO_RODA_DIREITA);
	glDeleteVertexArrays(1, &VAO_CORPO);
	// Finaliza a execu��o da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Fun��o de callback de teclado - s� pode ter uma inst�ncia (deve ser est�tica se
// estiver dentro de uma classe) - � chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

int setupGeometry()
{

	GLfloat vertices[] = {
		//x   y     z    r    g    b
		-0.8, -0.5, 0.0, 0.0, 0.0, 1.0,
		 0.8, -0.5, 0.0, 0.0, 0.0, 1.0,
		 0.8, 0.0,  0.0, 0.0, 0.0, 1.0,
		 0.6, 0.05, 0.0, 0.0, 0.0, 1.0,
		 0.3, 0.3, 0.0, 0.0, 0.0, 1.0,
		-0.3, 0.3,  0.0, 0.0, 0.0, 1.0,
		-0.3, 0.0, 0.0, 0.0, 0.0, 1.0, 
		-0.8, 0.0, 0.0, 0.0, 0.0, 1.0,
		-0.8, -0.5,  0.0, 0.0, 0.0, 1.0
	};

	GLuint VBO, VAO;


	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Atributo posi��o
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo cor
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

	return VAO;
}

int setupCircle(int secondWheel)
{
	GLfloat raio = 0.3;
	GLfloat doisPi = 2 * 3.14;

	// Aqui setamos as coordenadas x, y e z do c�rculo e as armazenamos de forma sequencial, j� visando mandar para o VBO (Vertex Buffer Objects)
	// Cada atributo do v�rtice (coordenada, cores, coordenadas de textura, normal, etc)
	// Pode ser arazenado em um VBO �nico ou em VBOs separados
	GLfloat vertices[(NLADOS+4) * 6];

	//
	for (int i = 0; i < NLADOS; i++) {
		if (secondWheel)	vertices[i * 6] = raio * sin(i * doisPi / NLADOS) + 0.5;
		else 	vertices[i * 6] = raio * sin(i * doisPi / NLADOS) - 0.5 ;

		vertices[i * 6 + 1] = raio * cos(i * doisPi / NLADOS)-0.5;
		vertices[i * 6 + 2] = 0.0;
		vertices[i * 6 + 3] = 1.0;
		vertices[i * 6 + 4] = 1.0;
		vertices[i * 6 + 5] = 1.0;
	}




	GLuint VBO, VAO;


	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//Atributo posi��o
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo cor
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0); 

	glBindVertexArray(0); 

	return VAO;
}

