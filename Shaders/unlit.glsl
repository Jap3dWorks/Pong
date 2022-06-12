#shader vertex
void vertex_main()
{
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0));

	gl_Position = Projection * View * model * vec4(aPos, 1.0);
}


#shader fragment
void main()
{
	FragColor = vec4(1.0, 0.0, 0.5, 1.0);
}