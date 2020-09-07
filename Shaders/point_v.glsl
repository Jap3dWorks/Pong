#version 460
layout (location = 0) in vec3 aPos;
//layout (location = 1) in vec3 aNormal;
//layout (location = 2) in vec2 aTexCoords;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;


void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0);
//    gl_PointSize = (1 - (gl_Position.z)) * 10.0;
    gl_PointSize = 100 / gl_Position.w;
//    gl_PointSize = 5;
}
