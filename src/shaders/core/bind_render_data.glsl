#ifndef BIND_RENDER_DATA_GLSL
#define BIND_RENDER_DATA_GLSL

layout (std140, binding=3) uniform RenderData {
    uint RenderWidth;
    uint RenderHeight;
    float ZNear;
    float ZFat;
    float Aspect;
};

#endif // BIND_RENDER_DATA_GLSL
