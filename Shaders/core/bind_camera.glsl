#ifndef BIND_CAMERA_GLSL
#define BIND_CAMERA_GLSL

layout (std140, binding=0) uniform CameraData {
    mat4 Projection;
    mat4 View;
    vec3 ViewPos;
    float FOV;
};

#endif // BIND_CAMERA_GLSL