#version 450 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec2 aTexCoords;
layout (location=2) in vec3 aNormal;

layout (std140, binding = 0) uniform ViewMatrices
{
	mat4 projection;
	mat4 view;
};


out vec2 TexCoords;
out vec3 WorldPos;
out vec3 Normal;

uniform mat4 model;

void main()
{
	TexCoords = aTexCoords;
	WorldPos = vec3(model * vec4(aPos, 1.f));

	Normal = mat3(model) * Normal;

	gl_Position = projection * view * vec4(WorldPos, 1.f);

}