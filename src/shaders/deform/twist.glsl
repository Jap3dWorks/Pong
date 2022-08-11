#ifndef TWIST_GLSL
#define TWIST_GLSL

/*
 * Twist raymarching_shaders point,
 * Use returned point to calculate a primitive sdf
*/
vec3 op_twist(in vec3 point) {
    const float k = 10.0;
    float c = cos(k*p.y);
    float s = sin(k*p.y);
    mat2 m = mat2(c, -s, s, c);
    return vec3(m*p.xz, p.y);
}

#endif
