#ifndef BEND_GLSL
#define BEND_GLSL

/*
 * Bend raymarching point,
 * use returned point to calculate a primitive sdf
*/
vec3 op_cheap_bend(in vec3 point) {
    const float k = 10.0;
    float c = cos(k*p.x);
    float s = sin(k*p.x);
    mat2 m = mat2(c, -s, s, c);
    return vec3(m*p.xy, p.z);
}

#endif
