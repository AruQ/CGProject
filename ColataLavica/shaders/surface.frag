#version 330 core

in vec2 TexCoords;
in float Red;

out vec4 color;

uniform sampler2D texture_diffuse1;

void main()
{   
    //float ambientStrength = 2.1f;
    //vec3 ambient = ambientStrength * vec3(1.0f,1.0f,1.0f);
    if (Red == 0.0f)
    {
        vec3 result =  vec3(texture(texture_diffuse1, TexCoords));
        color = vec4(result,1.0f);
    }
    else
    {
        color = vec4(1.0f, Red-0.01f,0.0f,1.0f);
    }
    //color = vec4(0.0f,0.0f,1.0f,1.0f);


    
}

