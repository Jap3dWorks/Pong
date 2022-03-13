#shader vertex
#version 450 core

layout(location=0) in vec4 position;
void main()
{
    gl_position = position;
}

#shader fragment
#version 330 core
layout(location=0) in vec4 color;

void main() {
    color = vec4(0.2, 0.0, 0.0, 1.0);
}
