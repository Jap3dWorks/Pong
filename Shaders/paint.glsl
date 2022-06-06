#shader vertex
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
