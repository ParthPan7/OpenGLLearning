#pragma once
#include "PointLight.h"
class SpotLight :
    public PointLight
{
public:
    SpotLight();

    SpotLight(GLfloat red, GLfloat blue, GLfloat green,
        GLfloat aIntensity, GLfloat dIntensity,
        GLfloat xPos, GLfloat yPos, GLfloat zPos,
        GLfloat xDir, GLfloat yDir, GLfloat zDir,
        GLfloat con, GLfloat lin, GLfloat exp,
        GLfloat edg);

    void UseLight(GLuint ambientIntensityLocation, GLuint ambientColourLocation,
        GLuint positionLocation, GLuint diffuseIntensityLocation, GLuint directionLocation,
        GLuint constantLocation, GLuint linearLocation, GLuint exponentLocation,
        GLuint edgeLocation);

    void SetFlash(glm::vec3 pos, glm::vec3 dir);

   ~SpotLight();

private:
    glm::vec3 direction;

    GLfloat edge, procEdge;
};

