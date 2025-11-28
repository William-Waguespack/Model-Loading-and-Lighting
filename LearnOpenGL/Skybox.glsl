#shader vertex
#version 330
layout (location = 0) in vec3 aPos;

out vec3 textureCoords;

uniform mat4 proj;
uniform mat4 vieww;

void main()
{
    vec4 pos = proj * vieww * vec4(aPos, 1.0f);
    gl_Position = pos.xyww;
    textureCoords = vec3(aPos.x, aPos.y, -aPos.z);
}



#shader fragment
#version 330
out vec4 FragColor;

in vec3 textureCoords;

uniform samplerCube skybox;

void main()
{
    
    FragColor = texture(skybox, textureCoords);
}

