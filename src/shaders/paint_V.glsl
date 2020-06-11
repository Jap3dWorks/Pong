#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;
out vec3 Normal;

uniform mat4 view;
uniform mat4 model;
uniform mat4 projection;

void main()
{
	mat3 normalMatrix = transpose(inverse(mat3(model)));
	gl_Position = projection * view * model * vec4(aPos, 1.0);  // if this line is not in this pos, shader fails :(
	vec3 view_normal = mat3(view) * normalize(normalMatrix * aNormal);
	TexCoords = vec2(view_normal) / 2 + 0.5;
}