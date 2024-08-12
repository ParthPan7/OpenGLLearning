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
    uniformDirectionalLight.uniformColour = glGetUniformLocation(shaderID, "directionalLight.colour");
    uniformDirectionalLight.uniformAmbientIntensity = glGetUniformLocation(shaderID, "directionalLight.ambientIntensity");
    uniformDirectionalLight.uniformDirection = glGetUniformLocation(shaderID, "directionalLight.direction");
    uniformDirectionalLight.uniformDiffuseIntensity = glGetUniformLocation(shaderID, "directionalLight.diffuseIntensity");
    uniformEyePosition = glGetUniformLocation(shaderID, "eyePosition");
    uniformSpecularIntensity = glGetUniformLocation(shaderID, "material.specularIntensity");
    uniformShininess = glGetUniformLocation(shaderID, "material.shininess");
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
