#version 450 core
layout (location = 0) out vec4 FragColor;

struct Directional{
	vec4 Direction;
	vec4 Color;
	vec4 Ambient;
};

struct Point{
	vec4 Position;
	vec4 Color;
};

layout (std140, binding = 1) uniform Lights{
	Directional directional;
	Point lights[5];
};

in VS_OUT{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
}fs_in;

uniform vec3 surfaceColor;
uniform vec3 viewPos;

uniform float glow;
uniform float specular;

void main()
{
	vec3 color = surfaceColor;
	vec3 dir_color = vec3(directional.Color);
	vec3 normal = normalize(fs_in.Normal);

	vec3 ambient = 0.1 * color + vec3(directional.Ambient);

	vec3 lighting = ambient;
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);

	// calculate directional
	vec3 lightDir = normalize(-vec3(directional.Direction));
	float diff = max(dot(lightDir, normal), 0.0);
	lighting += dir_color * diff * color;
	// specular
	vec3 halfDir = normalize(viewDir + lightDir);
	float spec = pow(max(dot(normal, halfDir), 0.0), glow);
	lighting += dir_color * spec * specular;

	// point lights
//	for(int i = 0; i < 5; i++)
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
//	FragColor = vec4(lighting, 1.0);
	FragColor = vec4(directional.Color);
}