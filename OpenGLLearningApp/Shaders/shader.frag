#version 330                    
                                
out vec4 color;                 
                                    
in vec4 vCol;                                    
in vec2 texCoord;
in vec3 Normal;
in vec3 FragPos;

struct DirectionalLight
{
    vec3 colour;
    float ambientIntensity;
    vec3 direction;
    float diffuseIntensity;
};

struct Material
{
    float specularIntensity;
    float shininess;
};

uniform sampler2D theTexture; 
uniform DirectionalLight directionalLight;
uniform Material material;
uniform vec3 eyePosition;

void main()                       
{                               
    vec4 ambientLight = vec4(directionalLight.colour, 1.0f) * directionalLight.ambientIntensity;  
    float diffuseFactor = max(dot(normalize(Normal), normalize(directionalLight.direction)), 0.0f);
    vec4 diffuseLight = vec4(directionalLight.direction, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;
    color = texture(theTexture, texCoord);
    
    vec4 specularLight = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    if(diffuseFactor > 0.0f)
    {
        vec3 FragToEyeVector = normalize(eyePosition - FragPos);
        vec3 reflectionVector = normalize(reflect(directionalLight.direction, normalize(Normal)));
        float specularFactor = dot(reflectionVector, FragToEyeVector);
        if(specularFactor > 0.0f)
        {
            specularFactor = pow(specularFactor, material.shininess);
            specularLight = vec4(directionalLight.colour * specularFactor * material.specularIntensity, 1.0f);
        }
    }    

    color = color * (ambientLight + diffuseLight + specularLight); 
}