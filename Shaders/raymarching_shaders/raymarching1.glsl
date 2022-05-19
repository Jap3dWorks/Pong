#shader vertex
#version 450

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

layout (std140, binding=0) uniform ViewMatrices {
    mat4 Projection;
    mat4 View;
    vec3 ViewPos;};

layout (std140, binding=2) uniform FrameData {
    double DeltaTime;
    double Time;
    uint Frame;
    float Fps;
};

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

out mat3 view_matrix;

void main() {
    vs_out.FragPos = vec3(aPos.x, aPos.y, aPos.z);
    view_matrix = inverse(mat3(View));
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.f);
}

#shader fragment
#version 450



