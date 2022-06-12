#shader vertex
void vertex_main()
{
	vs_out.TexCoords = aTexCoords;
	gl_Position = projection * view * model * vec4(aPos, 1);
}

#shader fragment
// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	FragColor = mix(texture(texture1, vs_in.TexCoords),
			texture(texture2, vs_in.TexCoords), 0.2);
}


