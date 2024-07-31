#include "Light.h"

Light::Light()
{
	colour = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.0f;
}

Light::Light(GLfloat red, GLfloat blue, GLfloat green, GLfloat aIntensity, GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity)
{
	colour = glm::vec3(red, blue, green);
	ambientIntensity = aIntensity;
	direction = glm::vec3(xDir, yDir, zDir);
	diffuseIntensity = dIntensity;
}

void Light::UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation, GLuint directionLocation, GLuint diffuseIntensityLocation)
{
	glUniform3f(ambientColourLocation, colour.x, colour.y, colour.z);
	glUniform1f(ambientIntensityLocation, ambientIntensity);
	glUniform3f(directionLocation, direction.x, direction.y, direction.z);
	glUniform1f(diffuseIntensityLocation, diffuseIntensity);
}

Light::~Light()
{
}
