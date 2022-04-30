#shader vertex
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

layout (std140, binding = 0) uniform ViewMatrices
{
	mat4 projection;
	mat4 view;
	vec3 viewPos;
};

layout (std140, binding=2) uniform FrameData
{
	uint frame;
	float fps;
};

out VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
}vs_out;

uniform mat4 model;

void main()
{
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
	vs_out.TexCoords = aTexCoords;
	mat3 normalMatrix = transpose(inverse(mat3(model)));
	vs_out.Normal = normalize(normalMatrix * aNormal);

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}


#shader fragment
#version 450 core
layout (location = 0) out vec4 FragColor;

layout (std140, binding = 0) uniform ViewMatrices {
	mat4 projection;
	mat4 view;
	vec3 viewPos;
};

layout (std140, binding=2) uniform FrameData {
	uint frame;
	float fps;
};

struct Directional{
	vec3 direction;
	vec3 color;
	vec3 ambient;
};

struct Point{
	vec3 Position;
	vec4 Color;
};

layout (std140, binding = 1) uniform LightsSourceBlock{
	Directional directional;
	Point lights[5];
};

in VS_OUT{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
}fs_in;

uniform vec3 surfaceColor;

uniform float glow;
uniform float specular;

void main()
{
	vec3 color = surfaceColor;
	vec3 dir_color = directional.color;
	vec3 normal = normalize(fs_in.Normal);

	vec3 ambient = 0.1 * color + directional.ambient;

	vec3 lighting = ambient;
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);

	// calculate directional
	vec3 lightDir = normalize(-directional.direction);
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

	FragColor = vec4(lighting, 1.0);
	//	FragColor = vec4(directional.color, 1.0);
}
