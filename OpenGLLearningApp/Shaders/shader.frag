#version 330                    
                                
out vec4 color;                 
                                    
in vec4 vCol;                                    
in vec2 texCoord;
in vec3 Normal;
in vec3 FragPos;

const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

struct Light
{
    vec3 colour;
    float ambientIntensity;
    float diffuseIntensity;
};

struct DirectionalLight
{
    Light base;
    vec3 direction;   
};

struct PointLight
{
    Light base;
    vec3 position;
    float constant;
    float linear;
    float exponent;
};

struct SpotLight
{
    PointLight base;
    vec3 direction;
    float edge;
};

struct Material
{
    float specularIntensity;
    float shininess;
};

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight  spotLights[MAX_SPOT_LIGHTS];

uniform int pointLightCount;
uniform int spotLightCount;

uniform sampler2D theTexture; 
uniform Material material;
uniform vec3 eyePosition;

vec4 CalculateLightByDirection(Light light, vec3 direction)
{
    vec4 ambientLight = vec4(light.colour, 1.0f) * light.ambientIntensity;  
    float diffuseFactor = max(dot(normalize(Normal), normalize(direction)), 0.0f);
    vec4 diffuseLight = vec4(light.colour, 1.0f) * light.diffuseIntensity * diffuseFactor;
    
    
    vec4 specularLight = vec4(0.0f, 0.0f, 0.0f, 0.0f);
    if(diffuseFactor > 0.0f)
    {
        vec3 FragToEyeVector = normalize(eyePosition - FragPos);
        vec3 reflectionVector = normalize(reflect(direction, normalize(Normal)));
        float specularFactor = dot(reflectionVector, FragToEyeVector);
        if(specularFactor > 0.0f)
        {
            specularFactor = pow(specularFactor, material.shininess);
            specularLight = vec4(light.colour * specularFactor * material.specularIntensity, 1.0f);
        }
    } 

    return (diffuseLight + ambientLight + specularLight);  
}


vec4 CalculateDirectionalLight()
{
    return CalculateLightByDirection(directionalLight.base, directionalLight.direction);
}

vec4 CalculatePointLight(PointLight pLight)
{
    vec3 direction = FragPos - pLight.position;
    float distance = length(direction);
    direction = normalize(direction);

    vec4 colour = CalculateLightByDirection(pLight.base, direction);

    float attentuation = pLight.exponent * distance * distance +
                             pLight.linear * distance + 
                             pLight.constant;
    return (colour/attentuation);
}

vec4 CalculatePointLights()
{
    vec4 totalColour = vec4(0.0f, 0.0f, 0.0f, 0.0f);

    for(int i=0; i<pointLightCount; i++)
    {
        totalColour += CalculatePointLight(pointLights[i]);
    }
    return totalColour;
}

vec4 CalculateSpotLight(SpotLight sLight)
{
    vec3 rayDirection = normalize(FragPos - sLight.base.position);
    float slFactor = dot(rayDirection, sLight.direction);

    if(slFactor > sLight.edge)
    {
        return CalculatePointLight(sLight.base) * (1.0f - (1.0f - slFactor) * (1.0f/(1.0f - sLight.edge)));
    }

    return vec4(0, 0 , 0 , 0);
}

vec4 CalculateSpotLights()
{
    vec4 totalColour = vec4(0, 0, 0, 0);
    for(int i=0; i<spotLightCount; i++)
    {
        totalColour += CalculateSpotLight(spotLights[i]);
    }
    return totalColour;
}

void main()                       
{                               
    vec4 aggregateColour = CalculateDirectionalLight();
    aggregateColour += CalculatePointLights();
    aggregateColour += CalculateSpotLights();
    color =  texture(theTexture, texCoord) * aggregateColour ;
}