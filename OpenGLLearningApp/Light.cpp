#include "Light.h"

Light::Light()
{
	colour = glm::vec3(1.0f, 1.0f, 1.0f);
	ambientIntensity = 1.0f;
}

Light::Light(GLfloat red, GLfloat blue, GLfloat green, GLfloat aIntensity, GLfloat dIntensity)
{
	colour = glm::vec3(red, blue, green);
	ambientIntensity = aIntensity;
	diffuseIntensity = dIntensity;
}



Light::~Light()
{
}
