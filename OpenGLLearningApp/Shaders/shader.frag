#version 330                    
                                
out vec4 color;                 
                                    
in vec4 vCol;                                    
in vec2 texCoord;
in vec3 Normal;

struct DirectionalLight
{
    vec3 colour;
    float ambientIntensity;
    vec3 direction;
    float diffuseIntensity;
};

uniform sampler2D theTexture; 
uniform DirectionalLight directionalLight;

void main()                       
{                               
    vec4 ambientLight = vec4(directionalLight.colour, 1.0f) * directionalLight.ambientIntensity;  
    float diffuseFactor = max(dot(normalize(Normal), normalize(directionalLight.direction)), 0.0f);
    vec4 diffuseLight = vec4(directionalLight.direction, 1.0f) * directionalLight.diffuseIntensity * diffuseFactor;
    color = texture(theTexture, texCoord);
    color = color * (ambientLight + diffuseLight); 
}