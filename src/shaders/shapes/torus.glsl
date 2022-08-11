#ifndef TORUS_GLSL
#define TORUS_GLSL

float sd_torus(in vec3 point, in float radius, in float thickness) {
    vec2 q = vec2(length(point.xz) - radius, point.y);
    return length(q) - thickness;
}

#endif // TORUS_GLSL
