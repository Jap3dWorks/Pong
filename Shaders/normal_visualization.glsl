#shader vertex

void main() {
    mat3 normal_matrix = mat3(transpose(inverse(view * model)));
    vs_out.normal = vec3(vec4(normal_matrix * aNormalm, 0.0));
    gl_Position = projection * view * vec4(aPos, 1.0);
}


#shader fragment

void main() {
    FragColor = vec4(1.0, 1.0, 0.0, 1.0);
}

#shader geometry

layout (line_strip, max_vertices=6) out;

const float MAGNITUDE = 0.2;

uniform mat4 projection;

void GenerateLine(int index)
{
    gl_Position = projection * gl_in[index].gl_Position;
    EmitVertex();
    gl_Position = projection * (gl_in[index].gl_Position + vec4(vs_in[index].normal, 0.0) * MAGNITUDE);
    EmitVertex();
    EndPrimitive();
}

void main() {
    GenerateLine(0); // first vertex normal
    GenerateLine(1); // second vertex normal
    GenerateLine(2); // third vertex normal
}
