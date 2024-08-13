#include "PointLight.h"

PointLight::PointLight() : Light()
{
	position = glm::vec3(0.0f, 0.0f, 0.0f);
	constant = 1.0f;
	exponent = 0.0f;
	linear   = 0.0f;
}

PointLight::PointLight(GLfloat red, GLfloat blue, GLfloat green, 
	GLfloat aIntensity, GLfloat dIntensity, 
	GLfloat xPos, GLfloat yPos, GLfloat zPos, 
	GLfloat con, GLfloat lin, GLfloat exp) : Light(red, green, blue, aIntensity, dIntensity)
{
	position = glm::vec3(xPos, yPos, zPos);
	constant = con;
	linear = lin;
	exponent = exp;
}

void PointLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation, 
	GLuint positionLocation, GLuint diffuseIntensityLocation,
	GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation)
{
	
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
	
	glUniform3f(positionLocation, position.x, position.y, position.z);
	glUniform1f(constantLocation, constant);
	glUniform1f(linearLocation,   linear);
	glUniform1f(exponentLocation, exponent);
}


PointLight::~PointLight()
{
}
