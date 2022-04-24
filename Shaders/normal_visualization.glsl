#shader vertex
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


#shader fragment
#version 460 core
out vec4 FragColor;

void main() {
    FragColor = vec4(1.0, 1.0, 0.0, 1.0);
}


#shader geometry
#version 460 core
layout (triangles) in;
layout (line_strip, max_vertices=6) out;

in VS_OUT {
    vec3 normal;
}gs_in[];

const float MAGNITUDE = 0.2;

uniform mat4 projection;

void GenerateLine(int index)
{
    gl_Position = projection * gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = projection * (gl_in[index].gl_Position + vec4(gs_in[index].normal, 0.0) * MAGNITUDE);
    EmitVertex();
    EndPrimitive();
}

void main() {
    GenerateLine(0); // first vertex normal
    GenerateLine(1); // second vertex normal
    GenerateLine(2); // third vertex normal
}
