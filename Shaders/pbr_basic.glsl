#shader vertex

void vertex_main()
{
	vs_out.TexCoords = aTexCoords;
	vs_out.WorldPos = vec3(model * vec4(aPos, 1.f));

	vs_out.Normal = mat3(model) * Normal;

	gl_Position = Projection * View * vec4(vs_out.WorldPos, 1.f);

}


#shader fragment

// material parameters
uniform sampler2D albedoMap;
uniform sampler2D normalMap;
uniform sampler2D metallicMap;
uniform sampler2D roughnessMap;
uniform sampler2D aoMap;

// lights
uniform vec3 pointLightPositions[5];
uniform vec3 pointLightColors[5];

uniform vec3 camPos;

const float PI = 3.14159265359;

vec3 getNormalFromMap()
{
	vec3 tangentNormal = texture(normalMap, vs_in.TexCoords).xyz * 2.0 -1.0;

	vec3 Q1 = dFdx(vs_in.WorldPos);
	vec3 Q2 = dFdy(vs_in.WorldPos);
	vec2 st1 = dFdx(vs_in.TexCoords);
	vec2 st2 = dFdy(vs_in.TexCoords);

	vec3 N = normalize(vs_in.Normal);
	vec3 T = normalize(Q1 * st2.t - Q2*st1.t);
	vec3 B = -normalize(cross(N, T));
	mat3 TBN = mat3(T, B, N);

	return normalize(TBN * tangentNormal);
}

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
	float a = roughness*roughness;
	float a2 =  a * a;
	float NdotH = max(dot(N, H), 0.0);
	float NdotH2 = NdotH * NdotH;

	float nom = a2;
	float denom = (NdotH2 * (a2 - 1.0) +1.0);
	denom = PI * denom * denom;

	return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
	float r = (roughness + 1);
	float k = (r * r) / 8.0;

	float nom = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
	float NdotV = max(dot(N, V), 0.0);
	float NdotL = max(dot(N, L), 0.0);

	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
	return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

void main()
{
	vec3 albedo = pow(texture(albedoMap, vs_in.TexCoords).rgb, vec3(2.2));
	float metallic = texture(metallicMap, vs_in.TexCoords).r;
	float roughness = texture(roughnessMap, vs_in.TexCoords).r;
	float ao = texture(aoMap, vs_in.TexCoords).r;

	vec3 N = getNormalFromMap();
	vec3 V = normalize(camPos - vs_in.WorldPos);

	// calculate reflectance
	vec3 F0 = vec3(0.04);
	F0 = mix(F0, albedo, metallic);

	// reflectance equation
	vec3 Lo = vec3(0.0);
	for (int i = 0; i<5; ++i)
	{
		// calculate per_light radiance
		vec3 L = normalize(pointLightPositions[i] - vs_in.WorldPos);
		vec3 H = normalize(V + L);
		float distances = length(pointLightPositions[i] - vs_in.WorldPos);
		float attenuation = 1.0 / (distances * distances);
		vec3 radiance = pointLightColors[i] * attenuation;

		// Cook-Torrance  brdf
		float NDF = DistributionGGX(N, H, roughness);
		float G = GeometrySmith(N, V, L, roughness);
		vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

		vec3 nominator = NDF * G * F;
		float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // prevent divide by 0
		vec3 specular = nominator / denominator;

		// KS equal is to fresnel
		vec3 KS = F;
		vec3 KD = vec3(1.0) - KS;

		KD *= 1.0 - metallic;

		// scale light by NdotL
		float NdotL = max(dot(N, L), 0.0);

		// add to outgoing radiance Lo
		Lo += (KD * albedo / PI + specular) * radiance * NdotL;
	}

	// ambient light
	vec3 ambient = vec3(0.03) * albedo * ao;

	vec3 color = ambient + Lo;

	// HDR tonemapping
	color = color / (color + vec3(1.0));
	// gamma correct
	color = pow(color, vec3(1.0 / 2.2));

	FragColor = vec4(color, 1.0);
}
