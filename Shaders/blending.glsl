#shader vertex

void main(){
    TexCoords = aTexCoords;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}


#shader fragment

uniform sampler2D texture1;

void main()
{
    FragColor = texture(texture1, TexCoords);
}
