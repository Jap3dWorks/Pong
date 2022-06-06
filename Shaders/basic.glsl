#shader vertex
void main()
{
    gl_position = aPos;
}

#shader fragment
void main() {
    FragColor = vec4(0.2, 0.0, 0.0, 1.0);
}
