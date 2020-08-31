#version 330 core
layout (location = 0) out vec4 FragColor;

in VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
}fs_in;

//struct Point
//{
//	vec3 Position;
//	vec3 Color;
//};

struct Directional
{
	vec3 Direction;
	vec3 Color;
	vec3 Ambient;
};

//uniform Point lights[4];
uniform Directional directional;
uniform vec3 surfaceColor;
uniform vec3 viewPos;

uniform float glow;
uniform float specular;

void main()
{
	vec3 color = surfaceColor;
	vec3 normal = normalize(fs_in.Normal);

	vec3 ambient = 0.1 * color + directional.Ambient;

	vec3 lighting = ambient;
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);

	// calculate directional
	vec3 lightDir = normalize(-directional.Direction);
	float diff = max(dot(lightDir, normal), 0.0);
	lighting += directional.Color * diff * color;
	// specular
	vec3 halfDir = normalize(viewDir + lightDir);
	float spec = pow(max(dot(normal, halfDir), 0.0), glow);
	lighting += directional.Color * spec * specular;

	// point lights
//	for(int i = 0; i < 4; i++)
//	{
//		lightDir = normalize(lights[i].Position - fs_in.FragPos);
//		diff = max(dot(lightDir, normal), 0.0);
//		vec3 result = lights[i].Color * diff * color;
//		
//		float distance = length(fs_in.FragPos - lights[i].Position);
//		
//		lighting += result * 1.0 / (distance * distance);
//	}

//	vec3 result = lighting;
	FragColor = vec4(lighting, 1.0);
}