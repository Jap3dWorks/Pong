#ifndef BOOLEAN_GLSL
#define BOOLEAN_GLSL

float op_union(float distance1, float distance2) {
    return min(distance1, distance2);
}

float op_subtraction(float distance1, float distance2) {
    return max(-distance1, distance2);
}

float op_intersection(float distance1, float distance2) {
    return max(distance1, distance2);
}

float op_smooth_union(float distance1, float distance2, float k) {
    float h = clamp(0.5 + 0.5 * (distance2 - distance1)/k, 0.0, 1.0);
    return mix(distance2, distance1, h) - k*h*(1.0 - h);
}

float op_smooth_subtraction(float distance1, float distance2, float k) {
    float h = clamp(0.5 - 0.5*(distance2 + distance1)/k, 0.0, 1.0);
    return mix(distance2, -distance1, h) + k*h*(1.0-h);
}

float op_smooth_intersection(float distance1,float distance2, float k) {
    float h = clamp(0.5 - 0.5*(distance2 - distance1)/k, 0.0, 1.0);
    return mix(distance2, distance1, h) + k*h*(1.0-h);
}




#endif // BOOLEAN_GLSL