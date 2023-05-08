#pragma once

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

class Sprite
{
public:
	Sprite() {}
	~Sprite() {}

	void initialize(int texID, int imgWidth, int imgHeight, float parallaxFactor = 0, int nAnimations = 1, int nFrames = 1);
	inline void setPosition(glm::vec3 pos) { this->position = pos; }
	inline glm::vec3 getPosition() { return position; }
	inline void moveLeft() { this->position.x -= speed.x; direction = 1; if (!airborne) iAnimation = 1; }
	inline void moveRight() { this->position.x += speed.x;  direction = 2; if (!airborne) iAnimation = 2; }
	//Se o personagem não está pulando, pula. A altura do pulo é regulada de acordo com o tamanho do sprite.
	inline void jump() { if (!airborne) { this->jumpSpeed = imgHeight / nAnimations * 1.25; airborne = true; } }
	inline void moveUp() { this->position.y += speed.y; }
	inline void moveDown() { this->position.y -= speed.y; }
	//Troca a animação do personagem para a animação parada na direção em que ele está quando não para de se mover
	inline void stop() { if(!airborne) iAnimation = 6 + direction; }

	inline void setScale(glm::vec3 dimensions) { this->dimensions = glm::vec3(dimensions.x / nFrames, dimensions.y / nAnimations, dimensions.z); }
	inline void setIAnimation(int iAnimation) { this->iAnimation = iAnimation; }
	void setShader(Shader* shader);
	void draw();
	//Sobrecarga do método update para fazer o scrolling
	void Sprite::update(float deltaX);
	void Sprite::update();

	void getAABB(glm::vec2& min, glm::vec2& max);


protected:
	GLuint VAO; //identificador do Vertex Array Object - Geometria
	glm::vec3 position;
	glm::vec3 dimensions;
	GLuint texID;
	int imgWidth, imgHeight;
	//Usado para calcular o quanto cada objeto vai se mover com o scrolling
	float parallaxFactor;
	int nAnimations, nFrames;
	int iFrame, iAnimation;
	//Por padrão, personagem começa virado pra direita.
	int direction = 2;
	float dx, dy;
	glm::vec2 speed;

	//Usado para calcular a velocidade do pulo e trocar a animação entre a de pulo e a de queda
	float jumpSpeed;
	bool airborne;

	//Usado para limitar o frame rate de animação
	double lastUpdateTime;
	float animationSpeed = 10.0f;
	


	Shader* shader;
};

