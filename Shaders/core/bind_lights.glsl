#ifndef BIND_LIGHTS_GLSL
#define BIND_LIGHTS_GLSL

#define MAX_LIGHTS 256

struct Directional {
    vec4 direction;
    vec4 color;
    vec4 ambient;
};

struct Point {
    vec4 Position;
    vec4 Color;
};

layout (std140, binding = 1) uniform LightsSourceBlock {
    Directional directional[MAX_LIGHTS];
    Point point_lights[MAX_LIGHTS];
    uint directional_count;
    uint point_count;
};

#endif  // BIND_LIGHTS_GLSL
