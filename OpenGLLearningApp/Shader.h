#pragma once
#include<GL/glew.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream> 

#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "CommonValues.h"

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

	void SetPointLights(PointLight* pLight, unsigned int lightCount);
	void SetSpotLights(SpotLight* sLight, unsigned int lightCount);

	Shader();
	~Shader();

private:
	int pointLightCount;
	GLuint uniformTransformModel, uniformProjectionModel, uniformViewModel, uniformEyePosition, 
		uniformSpecularIntensity, uniformShininess,
		shaderID;

	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformDirection;
	}uniformDirectionalLight;

	
	GLuint uniformPointLightCount;
	
	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;
	}uniformPointLight[MAX_POINT_LIGHTS];

	GLuint uniformSpotLightCount;
	
	struct {
		GLuint uniformColour;
		GLuint uniformAmbientIntensity;
		GLuint uniformDiffuseIntensity;

		GLuint uniformPosition;
		GLuint uniformConstant;
		GLuint uniformLinear;
		GLuint uniformExponent;

		GLuint uniformDirection;
		GLuint uniformEdge;
	}uniformSpotLight[MAX_SPOT_LIGHTS];



	void AddShader(GLuint theProgram, const char* shaderProgram, GLenum shaderType);
	void CompileShaders(const char* vShader, const char* fShader);
	std::string ReadFile(const char* fileLocation);
};

