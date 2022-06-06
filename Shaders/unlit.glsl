#shader vertex
void main()
{
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
	vs_out.TexCoords = aTexCoords;

	mat3 normalMatrix = transpose(inverse(mat3(model)));
	vs_out.Normal = normalize(normalMatrix * aNormal);

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}


#shader fragment
void main()
{
	FragColor = vec4(1.0, 0.0, 0.5, 1.0);
}