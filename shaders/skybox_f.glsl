# version 330 core
out vec3 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;
void main()
{
    FragColor = texture(skybox, TexCoords);
}