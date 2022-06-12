#shader vertex
void vertex_main()
{
    gl_Position = Projection * View * model * vec4(aPos, 1);
}


#shader fragment

uniform sampler2D texture1;

void main()
{
    vec4 texColor = texture(texture1, vs_in.TexCoords);
    if(texColor.a < 0.1)
    {
        discard;
    }
    FragColor = texColor;
}