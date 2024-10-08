// OpenGLLearningApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _USE_MATH_DEFINES
#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <stdio.h>
#include <vector>
#include<GL/glew.h>
#include<GLFW/glfw3.h>
#include <cmath>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>

#include "OpenGLLearningApp.h"
#include "Mesh.h"
#include "Shader.h"
#include "Window.h"
#include "Camera.h"
#include "Texture.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "CommonValues.h"
#include "Material.h"

GLuint transformModel, projection, view, ambientColour, ambientIntensity, eyePosition, 
directionLightVec, diffuseIntensity,
specularIntensity, shininess; // uniform variable that needs to be passed to shaders

std::vector<Mesh*> meshes;
std::vector<Shader*> shaders;
Camera camera;

Texture dirtTexture;
Texture brickTexture;
Texture plainTexture;

//Material definitions
Material shinyMaterial = Material(1.0f, 32.0f);

Material dullMaterial = Material(0.5f, 8.0f);

PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

//moving object around x-axis
const float xIncrement = 0.005f;
const float xMaxIncrementedValue = 0.2f;
bool direction = true;
float incrementX = 0.0f;
float toRadians = M_PI / 180.0f;
float incrementAngle = 0.00f;
// vertex shader location
static const char* vShaderFileLocation ="Shaders/shader.vert";

//fragment shader 
// vertex shader location 
static const char* fShaderFileLocation = "Shaders/shader.frag";
Window* window;

//Delta Time vars
GLfloat last = 0.0f;
GLfloat deltaTime = 0.0f;

void calcAverageNormals(GLint* indices, unsigned int indicesCount,  GLfloat* vertices, unsigned int verticesCount, 
    unsigned int vLength, unsigned int normalOffset)
{
    for (size_t i = 0; i < indicesCount; i += 3)
    {
        GLint in0 = indices[i] * vLength;
        GLint in1 = indices[i + 1] * vLength;
        GLint in2 = indices[i + 2] * vLength;

        glm::vec3 v1(vertices[in1]-vertices[in0], vertices[in1+1] - vertices[in0+1], vertices[in1+2]-vertices[in0+2]);
        glm::vec3 v2(vertices[in2]-vertices[in0], vertices[in2+1] - vertices[in0+1], vertices[in2+2]-vertices[in0+2]);
        
        glm::vec3 normal = glm::cross(v1, v2);
        normal = glm::normalize(normal);

        in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
        //update normals
        vertices[in0] += normal.x; vertices[in0 + 1]   += normal.y; vertices[in0 + 2] += normal.z;
        vertices[in1] += normal.x; vertices[in1 + 1]   += normal.y; vertices[in1 + 2] += normal.z;
        vertices[in2] += normal.x; vertices[in2 + 1]   += normal.y; vertices[in2 + 2] += normal.z;
    }

    for (size_t i=0; i < verticesCount/vLength; i++)
    {
        int inx = i * vLength + normalOffset;
        glm::vec3 vec(vertices[inx], vertices[inx+1], vertices[inx+2]);
        vec = glm::normalize(vec);
        vertices[inx] = vec.x; vertices[inx + 1] = vec.y; vertices[inx + 2] = vec.z;
    }
}

void CreateObjects()
{
    GLint indices[] = {
        0, 3, 1,
        1, 3, 2,
        2, 3, 0,
        0, 1, 2
    };
    GLfloat vertices[] = {
       // X     Y      Z      U      V    nx    ny  nz
     -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
     0.0f, -1.0f, 1.0f,  0.5f, 0.0f,  0.0f, 0.0f, 0.0f,
     1.0f, -1.0f, 0.0f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,
     0.0f,  1.0f, 0.0f,  0.5f, 1.0f,  0.0f, 0.0f, 0.0f
    };

    GLint floorIndices[] = {
        0, 2, 1,
        1, 2, 3
    };

    GLfloat floorVertices[] = {
        -10.0f, 0.0f, -10.0f,   0.0f, 0.0f,  0.0f, -1.0f, 0.0f,
        10.0f, 0.0f, -10.0f,   10.0f, 0.0f,  0.0f, -1.0f, 0.0f,
        -10.0f, 0.0f, 10.0f,   0.0f, 10.0f,  0.0f, -1.0f, 0.0f,
        10.0f, 0.0f, 10.0f,    10.0f, 10.0f, 0.0f, -1.0f, 0.0f  
    };

    calcAverageNormals(indices, 12, vertices, 32, 8, 5); 
    Mesh* mesh = new Mesh();
    meshes.push_back(mesh);
    meshes[0]->CreateMesh(indices, vertices, 12, 32);

    Mesh* mesh1 = new Mesh();
    meshes.push_back(mesh1);
    meshes[1]->CreateMesh(indices, vertices, 12, 32);
    
    Mesh* floorMesh = new Mesh();
    meshes.push_back(floorMesh);
    meshes[2]->CreateMesh(floorIndices, floorVertices, 6, 32);
}

void CreateShaders()
{
    Shader* shader = new Shader();
    shader->CreateShaderFromFiles(vShaderFileLocation, fShaderFileLocation);
    shaders.push_back(shader);
}

int main()
{   
    window = new Window();
    window->IntializeGLFWWindow();
    CreateObjects();
    CreateShaders();
    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f, 5.0f, 0.1f);
    dirtTexture = Texture("Textures/dirt.png");
    dirtTexture.LoadTexture();

    brickTexture = Texture("Textures/brick.png");
    brickTexture.LoadTexture();
    
    plainTexture = Texture("Textures/plain.png");
    plainTexture.LoadTexture();

    DirectionalLight mainLight = DirectionalLight(1.0f, 1.0f, 1.0f, 
                                                        0.1f, 0.0f, 
                                                1.0f, -1.0f, -1.0f);

    unsigned int pointLightCount = 0;
    pointLights[0] = PointLight(0.0f, 0.0f, 1.0f,
                                0.2f, 1.0f,
                                -4.0f, -1.0f, 0.0f,
                                0.3f, 0.2f, 0.1f);

    pointLightCount++;
    
    pointLights[1] = PointLight(0.0f, 1.0f, 0.0f,
                                 0.2f, 1.0f,
                                 4.0f, 2.0f, 0.0f,
                                0.3f, 0.2f, 0.1f);
    pointLightCount++;

    unsigned int spotLightCount = 0;
    spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
                                    1.0f, 1.0f,
                              0.0f,  0.0f, 0.0f,
                              0.0f, -1.0f, 0.0f,
                              0.3f, 0.2f, 0.1f,
                                        7.0f);

    spotLightCount++;

    transformModel = shaders[0]->GetUniformTransformModelLocation();
    projection = shaders[0]->GetUniformProjectionModelLocation();
    view = shaders[0]->GetUniformViewModelLocation();
    
    specularIntensity = shaders[0]->GetUniformSpecularIntensityLocation();
    shininess = shaders[0]->GetUniformShininessLocation();
    eyePosition = shaders[0]->GetUniformEyeLocation();

    glm::mat4 _projection = glm::perspective(45.0f, (GLfloat)window->GetBufferWidth() / (GLfloat)window->GetBufferHeight(), 0.1f, 100.0f);
    
    //Loop Until Window is closed
    while (!window->ShouldWindowClose())
    {
        GLfloat now = glfwGetTime();
        deltaTime = now - last;
        last = now;
        //Get + Handle user inputs
        glfwPollEvents();
        camera.keyControl(window->GetKeys(), deltaTime);
        camera.mouseControl(window->GetXChange(), window->GetYChange());
        if (direction)
        {
            incrementX += xIncrement;
        }
        else
        {
            incrementX -= xIncrement;
        }
        if (abs(incrementX) > xMaxIncrementedValue)
        {
            direction = !direction;
        }
        incrementAngle += 0.0125f;
        if (incrementAngle > 360.0f)
        {
            incrementAngle -= 360.0f;
        }
        glm::mat4 _transformModel(1.0f);
        
        
        
        _transformModel = glm::translate(_transformModel, glm::vec3(0.0f, 0.0f, -2.5f));
        _transformModel = glm::scale(_transformModel, glm::vec3(0.25f, 0.25f, 0.25f));
        //_transformModel = glm::rotate(_transformModel, incrementAngle * toRadians, glm::vec3(0.0f, 0.0f, 0.0f));
        
        
        //clear window
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shaders[0]->UseShader();
        shaders[0]->SetDirectionalLight(&mainLight);
        shaders[0]->SetPointLights(pointLights, pointLightCount);
        shaders[0]->SetSpotLights(spotLights, spotLightCount);
        glm::vec3 spotLightPosition = camera.GetCameraPosition();
        spotLightPosition.y -= 0.1f;
        spotLights[0].SetFlash(spotLightPosition, camera.GetCameraDirection());
            glUniformMatrix4fv(transformModel, 1, GL_FALSE, glm::value_ptr(_transformModel));
            glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(_projection));
            glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
            glm::vec3 cameraPosition = camera.GetCameraPosition();
            glUniform3f(eyePosition, cameraPosition.x, cameraPosition.y, cameraPosition.z);
            brickTexture.UseTexture();
            shinyMaterial.UseMaterial(specularIntensity, shininess);
            meshes[0]->RenderMesh();

            _transformModel = glm::mat4(1.0f);
            _transformModel = glm::translate(_transformModel, glm::vec3(0.0f, 1.0f, -2.5f));
            _transformModel = glm::scale(_transformModel, glm::vec3(0.25f, 0.25f, 0.25f));
            glUniformMatrix4fv(transformModel, 1, GL_FALSE, glm::value_ptr(_transformModel));
            glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(_projection));
            glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
            dirtTexture.UseTexture();
            dullMaterial.UseMaterial(specularIntensity, shininess);
            meshes[1]->RenderMesh();

            _transformModel = glm::mat4(1.0f);
            _transformModel = glm::translate(_transformModel, glm::vec3(0.0f, -0.3f, 0.0f));
            //_transformModel = glm::scale(_transformModel, glm::vec3(0.25f, 0.25f, 0.25f));
            glUniformMatrix4fv(transformModel, 1, GL_FALSE, glm::value_ptr(_transformModel));
            glUniformMatrix4fv(projection, 1, GL_FALSE, glm::value_ptr(_projection));
            glUniformMatrix4fv(view, 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
            dirtTexture.UseTexture();
            shinyMaterial.UseMaterial(specularIntensity, shininess);
            meshes[2]->RenderMesh();
            
        glUseProgram(0);
        window->WindowSwapBuffers();
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
