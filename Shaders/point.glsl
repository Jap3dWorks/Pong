#shader vertex

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    gl_PointSize = 100 / gl_Position.w;
}


#shader fragment

void main()
{
    FragColor = vec4(0.3, 0.6, 0.8, 1.0);
}
