#shader vertex
void vertex_main()
{
    gl_Position = vec4(aPos.x, aPos.y, -2.0, 1);

}

#shader fragment
void main() {
    FragColor = vec4(1.0, 1.0, 1.0, 1.0);

}
