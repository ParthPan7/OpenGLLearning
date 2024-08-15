#include "Shader.h"

void Shader::CreateShaderFromString(const char* vShader, const char* fShader)
{
    CompileShaders(vShader, fShader);
}

void Shader::CreateShaderFromFiles(const char* vShaderLocation, const char* fShaderLocation)
{
    std::string vertexShader = ReadFile(vShaderLocation);
    std::string fragmentShader = ReadFile(fShaderLocation);
    const char* vertexShaderProgram = vertexShader.c_str();
    const char* fragmentShaderProgram = fragmentShader.c_str();
    CompileShaders(vertexShaderProgram, fragmentShaderProgram);
}

void Shader::UseShader()
{
    glUseProgram(shaderID);
}

void Shader::ClearShader()
{
    if (shaderID != 0)
    {
        glDeleteShader(shaderID);
        shaderID = 0;
    }
    uniformProjectionModel = 0;
    uniformTransformModel = 0;
}

GLuint Shader::GetUniformTransformModelLocation()
{
    return uniformTransformModel;
}

GLuint Shader::GetUniformProjectionModelLocation()
{
    return uniformProjectionModel;
}

GLuint Shader::GetUniformViewModelLocation()
{
    return uniformViewModel;
}

GLuint Shader::GetUniformAmbientColourLocation()
{
    return uniformDirectionalLight.uniformColour;
}

GLuint Shader::GetUniformAmbientIntensityLocation()
{
    return uniformDirectionalLight.uniformAmbientIntensity;
}

GLuint Shader::GetUniformDirectionLocation()
{
    return uniformDirectionalLight.uniformDirection;
}

GLuint Shader::GetUniformDiffuseIntensityLocation()
{
    return uniformDirectionalLight.uniformDiffuseIntensity;
}

GLuint Shader::GetUniformShininessLocation()
{
    return uniformShininess;
}

GLuint Shader::GetUniformSpecularIntensityLocation()
{
    return uniformSpecularIntensity;
}

GLuint Shader::GetUniformEyeLocation()
{
    return uniformEyePosition;
}

void Shader::SetDirectionalLight(DirectionalLight* directionalLight)
{
    directionalLight->UseLight(uniformDirectionalLight.uniformAmbientIntensity, uniformDirectionalLight.uniformColour,
        uniformDirectionalLight.uniformDirection, uniformDirectionalLight.uniformDiffuseIntensity);
}

void Shader::SetPointLights(PointLight* pLight, unsigned int lightCount)
{
    if (lightCount > MAX_POINT_LIGHTS) { lightCount = MAX_POINT_LIGHTS; }

    glUniform1i(uniformPointLightCount, lightCount);

    for (size_t i=0; i<lightCount; i++)
    {
        pLight[i].UseLight(uniformPointLight[i].uniformAmbientIntensity, uniformPointLight[i].uniformColour,
            uniformPointLight[i].uniformPosition, uniformPointLight[i].uniformDiffuseIntensity,
            uniformPointLight[i].uniformConstant, uniformPointLight[i].uniformLinear, uniformPointLight[i].uniformExponent);
    }
}

void Shader::SetSpotLights(SpotLight* sLight, unsigned int lightCount)
{
    if (lightCount > MAX_SPOT_LIGHTS) { lightCount = MAX_SPOT_LIGHTS; }
    
    glUniform1i(uniformSpotLightCount, lightCount);

    for (size_t i = 0; i < lightCount; i++)
    {
        sLight[i].UseLight(uniformSpotLight[i].uniformAmbientIntensity, uniformSpotLight[i].uniformColour,
            uniformSpotLight[i].uniformPosition, uniformSpotLight[i].uniformDiffuseIntensity, uniformSpotLight[i].uniformDirection,
            uniformSpotLight[i].uniformConstant, uniformSpotLight[i].uniformLinear, uniformSpotLight[i].uniformExponent, uniformSpotLight[i].uniformEdge);
    }

}

Shader::Shader()
{
    shaderID = 0;
    uniformProjectionModel = 0;
    uniformTransformModel = 0;
    uniformViewModel = 0;
    uniformDirectionalLight.uniformColour = 0;
    uniformDirectionalLight.uniformAmbientIntensity = 0;
    uniformDirectionalLight.uniformDirection = 0;
    uniformDirectionalLight.uniformDiffuseIntensity = 0;
    uniformEyePosition = 0;
    uniformShininess = 0;
    uniformSpecularIntensity = 0;

    pointLightCount = 0;
}

Shader::~Shader()
{
    ClearShader();
}

void Shader::AddShader(GLuint theProgram, const char* shaderProgram, GLenum shaderType)
{
    GLuint theShader = glCreateShader(shaderType);

    const GLchar* theCode[1];
    theCode[0] = shaderProgram;

    GLint codeLength[1];
    codeLength[0] = strlen(shaderProgram);

    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        printf("Error compiling %d shader program : %s", shaderType, eLog);
        return;
    }

    glGetShaderiv(theShader, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetShaderInfoLog(theShader, sizeof(eLog), NULL, eLog);
        printf("Error validating %d Shader program : %s", shaderType, eLog);
        return;
    }

    glAttachShader(theProgram, theShader);
    return;
}


void Shader::CompileShaders(const char* vShader, const char* fShader)
{
    shaderID = glCreateProgram();

    if (!shaderID)
    {
        printf("Error creating shader program!\n");
        return;
    }
    AddShader(shaderID, vShader, GL_VERTEX_SHADER);
    AddShader(shaderID, fShader, GL_FRAGMENT_SHADER);

    GLint result = 0;
    GLchar eLog[1024] = { 0 };

    glLinkProgram(shaderID);
    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        printf("Problem while linking shader program %s \n", eLog);
        return;
    }

    glValidateProgram(shaderID);
    glGetProgramiv(shaderID, GL_VALIDATE_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(shaderID, sizeof(eLog), NULL, eLog);
        printf("Problem while validating a shader program %s \n", eLog);
        return;
    }
    uniformTransformModel = glGetUniformLocation(shaderID, "translateModel");
    uniformProjectionModel = glGetUniformLocation(shaderID, "projection");
    uniformViewModel = glGetUniformLocation(shaderID, "view");
    uniformDirectionalLight.uniformColour = glGetUniformLocation(shaderID, "directionalLight.base.colour");
    uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.base.ambientIntensity");
    uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
    uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.base.diffuseIntensity");
    uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");
    uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
    uniformShininess = glGetUniformLocation(shaderID, "material.shininess");

    uniformPointLightCount = glGetUniformLocation(shaderID, "pointLightCount");

    for (size_t i = 0; i < MAX_POINT_LIGHTS; i++)
    {
        char locBuff[100] = { '\0' };

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.colour", i);
        uniformPointLight[i].uniformColour = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.ambientIntensity", i);
        uniformPointLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].base.diffuseIntensity", i);
        uniformPointLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].position", i);
        uniformPointLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].constant", i);
        uniformPointLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].exponent", i);
        uniformPointLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "pointLights[%d].linear", i);
        uniformPointLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

    }

    uniformSpotLightCount = glGetUniformLocation(shaderID, "spotLightCount");

    for (size_t i = 0; i < MAX_SPOT_LIGHTS; i++)
    {
        char locBuff[100] = { '\0' };

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.colour", i);
        uniformSpotLight[i].uniformColour = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.ambientIntensity", i);
        uniformSpotLight[i].uniformAmbientIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.base.diffuseIntensity", i);
        uniformSpotLight[i].uniformDiffuseIntensity = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.position", i);
        uniformSpotLight[i].uniformPosition = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.constant", i);
        uniformSpotLight[i].uniformConstant = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.exponent", i);
        uniformSpotLight[i].uniformExponent = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].base.linear", i);
        uniformSpotLight[i].uniformLinear = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].edge", i);
        uniformSpotLight[i].uniformEdge = glGetUniformLocation(shaderID, locBuff);

        snprintf(locBuff, sizeof(locBuff), "spotLights[%d].direction", i);
        uniformSpotLight[i].uniformDirection = glGetUniformLocation(shaderID, locBuff);
    }
}

std::string Shader::ReadFile(const char* fileLocation)
{
    std::ifstream fileStream(fileLocation, std::ios::in);
    
    if (!fileStream.is_open())
    {
        printf("error reading file : %s ", fileLocation);
        return "";
    }
    std::string programcontent="";
    std::ostringstream oss;

    std::string line;
    while (std::getline(fileStream, line)) {
        oss << line << '\n'; 
    }

    fileStream.close(); 
    return oss.str(); 
}
