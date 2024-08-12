#pragma once
#include<GL/glew.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream> 

#include "DirectionalLight.h"
#include "PointLight.h"

using namespace std;
class Shader
{

public:
	void CreateShaderFromString(const char* vShader, const char* fShader);
	void CreateShaderFromFiles(const char* vShaderLocation, const char* fShaderLocation);
	void UseShader();
	void ClearShader();
	GLuint GetUniformTransformModelLocation();
	GLuint GetUniformProjectionModelLocation();
	GLuint GetUniformViewModelLocation();
	GLuint GetUniformAmbientColourLocation();
	GLuint GetUniformAmbientIntensityLocation();
	GLuint GetUniformDirectionLocation();
	GLuint GetUniformDiffuseIntensityLocation();
	GLuint GetUniformShininessLocation();
	GLuint GetUniformSpecularIntensityLocation();
	GLuint GetUniformEyeLocation();

	void SetDirectionalLight(DirectionalLight* directionalLight);

	Shader();
	~Shader();

private:
	GLuint uniformTransformModel, uniformProjectionModel, uniformViewModel, uniformEyePosition, 
		uniformSpecularIntensity, uniformShininess,
		shaderID;

	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDirection;
		GLuint uniformDiffuseIntensity;
	}uniformDirectionalLight;

	void AddShader(GLuint theProgram, const char* shaderProgram, GLenum shaderType);
	void CompileShaders(const char* vShader, const char* fShader);
	std::string ReadFile(const char* fileLocation);
};

