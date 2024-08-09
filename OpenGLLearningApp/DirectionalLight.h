#pragma once
#include "Light.h"
class DirectionalLight :
    public Light
{
public:
    DirectionalLight();
    ~DirectionalLight();
    DirectionalLight(GLfloat red, GLfloat blue, GLfloat green, GLfloat aIntensity, GLfloat xDir, GLfloat yDir, GLfloat zDir, GLfloat dIntensity);
    void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation, GLuint directionLocation, GLuint diffuseIntensityLocation);

private:
    glm::vec3 direction;
};

