#pragma once
#include "Light.h"
class PointLight :
    public Light
{
public:
    PointLight();

    PointLight(GLfloat red, GLfloat blue, GLfloat green, 
        GLfloat aIntensity, GLfloat dIntensity,
        GLfloat xPos, GLfloat yPos, GLfloat zPos,
        GLfloat con, GLfloat lin, GLfloat exp); 

    void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation, 
        GLuint positionLocation, GLuint diffuseIntensityLocation,
        GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation);
    
    ~PointLight();

protected:
    glm::vec3 position;

    GLfloat constant, linear, exponent;

};

