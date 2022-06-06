#shader vertex
void main() {
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
	vs_out.TexCoords = aTexCoords;

	mat3 normalMatrix = transpose(inverse(mat3(model)));
	vs_out.Normal = normalize(normalMatrix * aNormal);

	gl_Position = Projection * View * model * vec4(aPos, 1.0);
}


#shader fragment
uniform sampler2D diffuseTexture;
uniform vec3 viewPos;

void main()
{
	vec3 color = texture(diffuseTexture, fs_in.TexCoords).rgb;
	vec3 normal = normalize(fs_in.Normal);

	vec3 ambient = 0.0 * color;

	vec3 lighting = vec3(0.0);
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	for(int i = 0; i < point_count; i++)
	{
		vec3 lightDir = normalize(point_lights[i].Position.xyz - fs_in.FragPos);
		float diff = max(dot(lightDir, normal), 0.0);
		vec3 result = point_lights[i].Color.xyz * diff * color;

		float distance = length(fs_in.FragPos - point_lights[i].Position.xyz);
		result *= 1.0 / (distance * distance);
		lighting += result;
	}

	vec3 result = ambient + lighting;
	float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
	if (brightness > 1.0)
	BightColor = vec4(result, 1.0);
	else
	BightColor = vec4(0.0, 0.0, 0.0, 1.0);
	FragColor = vec4(result, 1.0);
}