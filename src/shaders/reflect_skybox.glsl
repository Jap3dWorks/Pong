#shader vertex

void vertex_main()
{
    vs_out.TexCoords = aPos;
    vec4 pos = Projection * View * vec4(aPos, 1.0);
    gl_Position = pos.xyww;
}


#shader fragment
uniform samplerCube skybox;

void main()
{
    FragColor = texture(skybox, vs_in.TexCoords);
}