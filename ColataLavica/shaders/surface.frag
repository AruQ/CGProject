#version 330 core

in vec2 TexCoords;

out vec4 color;

uniform sampler2D texture_diffuse1;

void main()
{   
float ambientStrength = 2.1f;
vec3 ambient = ambientStrength * vec3(1.0f,1.0f,1.0f);
vec3 result = ambient * vec3(texture(texture_diffuse1, TexCoords));
color = result;


    
}
 
