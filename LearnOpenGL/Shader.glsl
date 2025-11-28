#shader vertex
#version 330
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 texture;
layout (location = 2) in vec3 aNormal;

out vec2 textureCoords;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    textureCoords = texture;
    Normal = aNormal;

    FragPos = (model * vec4(aPos, 1.0)).xyz;

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}



#shader fragment
#version 330
out vec4 FragColor;

in vec2 textureCoords;
in vec3 FragPos;
in vec3 Normal;

uniform float Time;
uniform vec4 color1;
uniform vec4 color2;
uniform sampler2D texture_diffuse;

float inverseLerp(float v, float minValue, float maxValue)
{
    return (v - minValue) / (maxValue - minValue);
}

float remap(float v, float inMin, float inMax, float outMin, float outMax)
{
    float t = inverseLerp(v, inMin, inMax);
    return mix(outMin, outMax, t);
}


void main()
{
    
    float t1 = remap(sin(textureCoords.y * 400 + Time * 10), -1, 1, 0.9, 1);
    float t2 = remap(sin(-textureCoords.y * 50 + Time * 2), -1, 1, 0.8, 1);
    vec3 color = texture2D(texture_diffuse, textureCoords).xyz * t1 * t2;

    FragColor = vec4(color, 1.0);
    
}

