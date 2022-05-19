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


float hash1(float n) {
    return fract(sin(n) * 43758.5453);
}

vec2 hash2(vec2 p) {
    p=vec2(dot(p, vec2(127.1, 311.7)), dot(p, vec2(269.5, 183.3)));
    return fract(sin(p)) * 43758.5453;
}

vec4 voronoi(in vec2 x, in float w) {
    vec2 n = floor(x);
    vec2 f = fract(x);

    vec4 m = vec4(8.0, 0.0, 0.0, 0.0);
    for(int j=-2; j<=2; j++) {
        for(int i=-2; i<=2; i++) {
            vec2 g = vec2(float(i), float(j));
            vec2 o = hash2(n + g);

            // animate
            o = 0.5 + 0.5*sin(fTime + 6.2831*o);

            // distance to cell
            float d = length(g-f+o);

            // cell color
            vec3 col = 0.5 + 0.5*sin(hash1(dot(n+g, vec2(7.0, 113.0)))*2.5 + 3.5 + vec3(2.0, 3.0, 0.0));
            col = col*col;

            // smooth
            float h = smoothstep(-1.0, 1.0, (m.x-d) / w);
            m.x = mix(m.x, d, h) - h*(1.0-h)*w/(1.0+3.0*w);
            m.yzw = mix(m.yzw, col, h) - h*(1.0-h)*w/(1.0+3.0*w);
        }
    }
    return m;
}


void main() {
    vec3 r_direction = cam_dt.view_matrix * normalize(
    vec3(aspect_coords, cam_dt.z_camera_ray));

    //


    // gamma
    col = pow(col, vec3(0.4545));
    FragColor = vec4(col, 1.0);
}
