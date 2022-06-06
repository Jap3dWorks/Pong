#shader vertex

void main()
{
    TexCoords = aPos;
    vec4 pos = Projection * View * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}


#shader fragment

uniform samplerCube skybox;

void main()
{
    float ratio = 1.00/ 1.52; // air / glass reflactive index
    vec3 I = normalize / (Position - cameraPos);
    vec3 R = refract(I, normalize(Normal), ratio);

    FragColor = vec4(texture(skybox, R).rgb, 1.0);
}
