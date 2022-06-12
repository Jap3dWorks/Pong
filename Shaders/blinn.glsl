#shader vertex

void vertex_main() {
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
	vs_out.TexCoords = aTexCoords;
	mat3 normalMatrix = transpose(inverse(mat3(model)));
	vs_out.Normal = normalize(normalMatrix * aNormal);

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}


#shader fragment

uniform vec3 surfaceColor;
uniform float glow;
uniform float specular;

void main() {
	vec3 color = surfaceColor;
	vec3 dir_color = vec3(directional[0].color); // acumulate directionals
	vec3 normal = normalize(fs_in.Normal);

	vec3 ambient = 0.1 * color + vec3(directional[0].ambient);

	vec3 lighting = ambient;
	vec3 viewDir = normalize(vec3(viewPos) - fs_in.FragPos);

	// calculate directional
	vec3 lightDir = -normalize(vec3(directional[0].direction));
	float diff = max(dot(lightDir, normal), 0.0);
	lighting += dir_color * diff * color;

	// specular
	vec3 halfDir = normalize(viewDir + lightDir);
	float spec = pow(max(dot(normal, halfDir), 0.0), glow);
	lighting += dir_color * spec * specular;

//	 point lights
	for(int i = 0; i < point_count; i++) {
		lightDir = normalize(point_lights[i].Position.xyz - fs_in.FragPos);
		diff = max(dot(lightDir, normal), 0.0);
		vec3 result = point_lights[i].Color.xyz * diff * color;

		float distance = length(fs_in.FragPos - point_lights[i].Position.xyz);

		lighting += result * 1.0 / (distance * distance);
	}

	FragColor = vec4(lighting, 1.0);
}
