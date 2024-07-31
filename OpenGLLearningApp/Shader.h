#pragma once
#include<GL/glew.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream> 
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
	Shader();
	~Shader();

private:
	GLuint uniformTransformModel, uniformProjectionModel, uniformViewModel, uniformAmbientColour, uniformAmbientIntensity, uniformDirection, uniformDiffuseIntensity, shaderID;
	void AddShader(GLuint theProgram, const char* shaderProgram, GLenum shaderType);
	void CompileShaders(const char* vShader, const char* fShader);
	std::string ReadFile(const char* fileLocation);
};

