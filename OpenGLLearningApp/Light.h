#pragma once
#include<glm/glm.hpp>
#include<GL/glew.h>

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat blue, GLfloat green, GLfloat aIntensity, GLfloat dIntensity);
	~Light();

protected:
	glm::vec3 colour;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;
};

