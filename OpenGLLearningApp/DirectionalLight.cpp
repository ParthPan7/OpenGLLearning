#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
{
	direction = glm::vec3(0.0f, -1.0f, 0.0f);
}

DirectionalLight::~DirectionalLight()
{
}

DirectionalLight::DirectionalLight(GLfloat red, GLfloat blue, GLfloat green, GLfloat aIntensity, GLfloat dIntensity,
	GLfloat xDir, GLfloat yDir, GLfloat zDir) : Light(red, green, blue, aIntensity, dIntensity)
{
	direction = glm::vec3(xDir, yDir, zDir);
}

void DirectionalLight::UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation, GLuint directionLocation, GLuint diffuseIntensityLocation)
{
	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}
