#shader vertex
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

layout (std140, binding = 0) uniform ViewMatrices
{
	mat4 projection;
	mat4 view;
	vec3 viewPos;
};


out vec2 TexCoords;
out vec3 Normal;

uniform mat4 model;

void main()
{
	mat3 normalMatrix = transpose(inverse(mat3(model)));
	gl_Position = projection * view * model * vec4(aPos, 1.0);  // if this line is not in this pos, Shader fails :(
	vec3 view_normal = mat3(view) * normalize(normalMatrix * aNormal);
	TexCoords = vec2(view_normal) / 2 + 0.5;
}


#shader fragment
#version 330 core
layout (location = 0) out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture1;

void main()
{
	FragColor = texture(texture1, TexCoords);
}
