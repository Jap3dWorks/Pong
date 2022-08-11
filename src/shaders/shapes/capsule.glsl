#ifndef CAPSULE_GLSL
#define CAPSULE_GLSL

float sd_capsule(in vec3 p, in vec3 a, in vec3 b, in float r) {
    vec3 pa = p - a, ba = b - a;
    float h = clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0);
    return length(pa - ba * h) - r;
}

#endif // CAPSULE_GLLS
