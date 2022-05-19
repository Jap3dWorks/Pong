#shader vertex
#version 450

#include "primitives/capsule.glsl"

void main() {
    // vertex shader
    include_this();
}

#shader fragment
#include "noises/voronoi.glsl"

void main() {
    // fragment shader
}

#shader geometry
void main() {
    // geometry shader
}