#shader vertex
void vertex_main()
{
    gl_Position = Projection * View * model * vec4(aPos, 1);
}

#shader fragment
void main() {
    FragColor = vec4(1.0, 1.0, 1.0, 1.0);
}
