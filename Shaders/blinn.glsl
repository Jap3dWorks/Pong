#shader vertex
#version 450 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

layout (std140, binding = 0) uniform ViewMatrices {
	mat4 projection;
	mat4 view;
	vec4 viewPos;
};

layout (std140, binding=2) uniform FrameData {
	double delta_time;
	double time;
	uint frame_count;
	float fps;
};

out VS_OUT {
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
}vs_out;

uniform mat4 model;

void main() {
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
	vs_out.TexCoords = aTexCoords;
	mat3 normalMatrix = transpose(inverse(mat3(model)));
	vs_out.Normal = normalize(normalMatrix * aNormal);

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}


#shader fragment
#version 450 core

#define MAX_LIGHTS 256

layout (location = 0) out vec4 FragColor;

layout (std140, binding = 0) uniform ViewMatrices {
	mat4 projection;
	mat4 view;
	vec4 viewPos;
};

struct Directional {
	vec4 direction;
	vec4 color;
	vec4 ambient;
};

struct Point {
	vec4 Position;
	vec4 Color;
};

layout (std140, binding = 1) uniform LightsSourceBlock {
	Directional directional[MAX_LIGHTS];
	Point point_lights[MAX_LIGHTS];
	uint directional_count;
	uint point_count;
};

layout (std140, binding=2) uniform FrameData {
	double delta_time;
	double time;
	uint frame_count;
	float fps;
};

in VS_OUT{
	vec3 FragPos;
	vec3 Normal;
	vec2 TexCoords;
}fs_in;

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
	for(int i = 0; i < point_count; i++)
	{
		lightDir = normalize(point_lights[i].Position.xyz - fs_in.FragPos);
		diff = max(dot(lightDir, normal), 0.0);
		vec3 result = point_lights[i].Color.xyz * diff * color;

		float distance = length(fs_in.FragPos - point_lights[i].Position.xyz);

		lighting += result * 1.0 / (distance * distance);
	}

	FragColor = vec4(lighting, 1.0);
}
