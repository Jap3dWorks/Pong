#ifndef ROUND_GLSL
#define ROUND_GLSL

float op_round(in float distance, in float radius) {
    return distance - radius;
}

#endif // ROUND_GLSL
