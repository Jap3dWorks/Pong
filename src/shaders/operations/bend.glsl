#ifndef BEND_GLSL
#define BEND_GLSL

vec3 op_cheap_bend(in vec3 point, in float bend) {
    float c = cos(bend*p.x);
    float s = sin(bend*p.x);
    mat2 m = mat2(c, -s, s, c);
    return vec3(m*p.xy, p.z);
}

#endif // BEND_GLSL