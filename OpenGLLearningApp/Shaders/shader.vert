#version 330                    
                                
layout (location=0) in vec3 pos; 
layout (location=1) in vec2 tex;   
layout (location=2) in vec3 normal;

uniform mat4 translateModel;                                    
uniform mat4 projection;                                    
uniform mat4 view;                            

out vec4 vCol;                                       
out vec2 texCoord;      
out vec3 Normal;

void main()                       
{                                   
    gl_Position =  projection * view * translateModel * vec4(pos, 1.0); 
    vCol = vec4(clamp(pos, 0.0, 1.0),1.0);                                                
    texCoord = tex;

    Normal = mat3(transpose(inverse(translateModel))) * normal;
}
