#version 450 core
layout (location = 0) in vec3 aPos;

layout (std140, binding = 0) uniform ViewMatrices
{
    mat4 projection;
    vec3 viewPos;
};

out vec3 TexCoords;

uniform mat4 view;

void main()
{
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}