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

uniform vec4 color1;
uniform vec4 color2;
uniform sampler2D Texture0;



vec3 red = vec3(1.0, 0.0, 0.0);
vec3 blue = vec3(0.0, 0.0, 1.0);
vec3 white = vec3(1.0, 1.0, 1.0);
vec3 black = vec3(0.0, 0.0, 0.0);
vec3 yellow = vec3(1.0, 1.0, 0.0);

void main() {
    vec3 colour = vec3(0.75);
    vec2 resolution = vec2(1920, 1920);
    // grid
    vec2 center = textureCoords - 0.5;
    vec2 cell = fract(center * resolution / 100.0);
    cell = abs(cell - 0.5);
    float distToCell = 1.0 - 2.0 * max(cell.x, cell.y);

    float cellLine = smoothstep(0.0, 0.05, distToCell);

    float xAxis = smoothstep(0.0, 0.002, abs(textureCoords.y - 0.5));
    float yAxis = smoothstep(0.0, 0.002, abs(textureCoords.x - 0.5));

    // Lines
    vec2 pos = center * resolution / 100.0;
    float value1 = pos.x;
    float value2 = fract(pos.x);
    float functionLine1 = smoothstep(0.0, 0.075, abs(pos.y - value1));
    float functionLine2 = smoothstep(0.0, 0.075, abs(pos.y - value2));

    colour = mix(black, colour, cellLine);
    colour = mix(blue, colour, xAxis);
    colour = mix(blue, colour, yAxis);
    colour = mix(yellow, colour, functionLine1);
    colour = mix(red, colour, functionLine2);

    //vec3 wsNormal = normalize(cross(dFdx(FragPos), dFdy(FragPos)));

    FragColor = vec4(colour, 1.0);
}

