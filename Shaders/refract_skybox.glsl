#shader vertex
#version 450 core
layout (location = 0) in vec3 aPos;

layout (std140, binding = 0) uniform ViewMatrices
{
    mat4 projection;
    vec3 viewPos;
};

out vec3 TexCoords;

uniform mat4 view;

void main()
{
    TexCoords = aPos;
    vec4 pos = projection * view * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}


#shader fragment
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
