#version 450 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

void main()
{
    float ratio = 1.00/ 1.52; // air / glass reflactive index
    vec3 I = normalize / (Position - cameraPos);
    vec3 R = refract(I, normalize(Normal), ratio);

    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}