#version 460 core

layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;

layout (std140, binding = 0) uniform ViewMatrices
{
    mat4 projection;
    mat4 view;
    vec3 viewPos;
};

out VS_OUT {
    vec3 normal;
} vs_out;

uniform mat4 model;

void main() {
    mat3 normal_matrix = mat3(transpose(inverse(view * model)));
    vs_out.normal = vec3(vec4(normal_matrix * aNormalm, 0.0));
    gl_Position = projection * view * vec4(aPos, 1.0);
}
