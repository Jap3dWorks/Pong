#shader vertex
void main()
{
	TexCoords = aTexCoords;
	gl_Position = projection * view * model * vec4(aPos, 1);
}

#shader fragment
// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	FragColor = mix(texture(texture1, TexCoords),
			texture(texture2, TexCoords), 0.2);
}


