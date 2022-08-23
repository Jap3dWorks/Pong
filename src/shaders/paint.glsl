#shader vertex
void vertex_main()
{
	mat3 normalMatrix = transpose(inverse(mat3(model)));
	gl_Position = projection * view * model * vec4(aPos, 1.0);  // if this line is not in this index, shader fails :(
	vec3 view_normal = mat3(view) * normalize(normalMatrix * aNormal);
	vs_out.TexCoords = vec2(view_normal) / 2 + 0.5;
}


#shader fragment

uniform sampler2D texture1;

void main()
{
	FragColor = texture(texture1, vs_in.TexCoords);
}
