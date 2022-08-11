#ifndef SPHERE_GLSL
#define SPHERE_GLSL

float sd_sphere(in vec3 point, in float radius) {
    return length(point) - radius;

}

#endif  // SPHERE_GLSL
