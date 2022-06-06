#ifndef BIND_FRAME_DATA_GLSL
#define BIND_FRAME_DATA_GLSL

layout (std140, binding=2) uniform FrameData {
    double DeltaTime;
    double Time;
    uint Frame;
    float Fps;
};

#endif // BIND_FRAME_DATA_GLSL