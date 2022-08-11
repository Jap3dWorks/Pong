#ifndef DISPLACEMENT_GLSL
#define DISPLACEMENT_GLSL

vec3 op_displace(in vec3 point, in vec3 displacement) {
    return point + displacement;
}


float op_sin_displace(in float distance, in vec3 point, in vec3 fase, in vec3 frequency, in float magnitude) {
    return distance + sin(point.x * frequency.x + fase.x) *
                      sin(point.y * frequency.y + fase.y) *
                      sin(point.z * frequency.z + fase.z) * magnitude;
}

#endif  // DISPLACEMENT_GLSL

