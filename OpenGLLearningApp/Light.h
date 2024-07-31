#pragma once
#include<glm/glm.hpp>
#include<GL/glew.h>

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat blue, GLfloat green, GLfloat aIntensity, GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity);
	void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation, GLuint directionLocation, GLuint diffuseIntensityLocation);
	~Light();

private:
	glm::vec3 colour;
	GLfloat ambientIntensity;

	glm::vec3 direction;
	GLfloat diffuseIntensity;
};

