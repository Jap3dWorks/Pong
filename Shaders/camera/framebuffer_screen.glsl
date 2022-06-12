#shader vertex
void vertex_main ()
{
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
}


#shader fragment
uniform sampler2D screenTexture;

void main()
{
    vec3 col = texture(screenTexture, vs_in.TexCoords).rgb;
    FragColor = vec4(col, 1.0);
//    FragColor = vec4(vs_in.TexCoords.x, vs_in.TexCoords.y, 0.0, 1.0);

}