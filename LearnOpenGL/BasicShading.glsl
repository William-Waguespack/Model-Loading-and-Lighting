#shader vertex
#version 330
layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;
layout(location = 3) in vec3 aTangent;
layout(location = 4) in vec3 aBitangent;
layout(location = 5) in ivec4 aBoneIDs;
layout(location = 6) in vec4 aBoneWeights;

out vec2 TexCoords;
out vec3 FragPos;
out vec3 Normal;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main()
{
    TexCoords = aTexCoords;
    Normal = mat3(transpose(inverse(model))) * aNormal;

    FragPos = (model * vec4(aPosition, 1.0)).xyz;

    gl_Position = projection * view * model * vec4(aPosition, 1.0);
}



#shader fragment
#version 330
out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;


uniform vec3 viewPos;
uniform vec3 lightDir;

uniform vec3 lightAmbient;
uniform vec3 lightDiffuse;
uniform vec3 lightSpecular;



uniform sampler2D texture_diffuse1;

uniform sampler2D texture_specular1;
uniform sampler2D texture_normal1;
uniform sampler2D texture_height1;
uniform sampler2D texture_emissive1;

void main()
{
    // Normalize input vectors
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 lightDirNorm = normalize(-lightDir); // light direction TO the surface

    // Ambient
    vec3 ambient = lightAmbient * texture(texture_diffuse1, TexCoords).rgb;

    // Diffuse
    float diff = max(dot(norm, lightDirNorm), 0.0);
    vec3 diffuse = lightDiffuse * diff * texture(texture_diffuse1, TexCoords).rgb;

    // Specular (Blinn-Phong)
    vec3 halfwayDir = normalize(viewDir + lightDirNorm);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 32);
    
    vec3 specular = lightSpecular * spec * texture(texture_specular1, TexCoords).rgb;

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}

