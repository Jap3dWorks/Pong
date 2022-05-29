#shader vertex

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

layout (std140, binding=0) uniform CameraData {
    mat4 Projection;
    mat4 View;
    vec3 ViewPos;
    float FOV;
};

layout (std140, binding=2) uniform FrameData {
    double DeltaTime;
    double Time;
    uint Frame;
    float Fps;
};

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

out CAM_DT {
    mat3 view_matrix;
    float z_camera_ray;
} cam_dt;

void main() {
    cam_dt.z_camera_ray = -abs(cos(FOV/2.0) / sin(FOV/2.0));
    vs_out.FragPos = vec3(aPos.x, aPos.y, aPos.z);
    cam_dt.view_matrix = inverse(mat3(View));
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.f);
}


#shader fragment
#define PI 3.141592653589793f

layout (std140, binding=0) uniform CameraData {
    mat4 Projection;
    mat4 View;
    vec3 ViewPos;
    float FOV;
};

layout (std140, binding=2) uniform FrameData {
    double DeltaTime;
    double Time;
    uint Frame;
    float Fps;
};

layout (std140, binding=3) uniform RenderData {
    uint RenderWidth;
    uint RenderHeight;
    float ZNear;
    float ZFat;
    float Aspect;
};

layout (location=0) out vec4 FragColor;

in VS_OUT{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
}vs_in;

in CAM_DT {
    mat3 view_matrix;
    float z_camera_ray;
} cam_dt;

float fTime = float(Time);

#include "textures/voronoi.glsl"


void main() {
    vec2 aspect_coords = vec2(vs_in.FragPos.x, vs_in.FragPos.y / Aspect);
    vec4 v = smooth_voronoi(aspect_coords * 6.0, 0.05);

    // gamma
    vec3 col = sqrt(v.yzw);
    FragColor = vec4(col, 1.0);
}
