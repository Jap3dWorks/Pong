#shader vertex
#version 450

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

out mat3 view_matrix;
// TODO: calc camera z_ray_value

void main() {
    vs_out.FragPos = vec3(aPos.x, aPos.y, aPos.z);
    view_matrix = inverse(mat3(View));
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.f);
}


#shader fragment
#version 450

#define RAYMARCH_STEPS 256
#define HW_PERFORMANCE
// https://digitalfreepen.com/2017/06/23/fast-mostly-consistent.html

#ifdef HW_PERFORMANCE
#define AA 1
#else
#define AA 3
#endif

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

in mat3 view_matrix;

float fTime = float(Time);

float sdCutSphere(
    in vec3 point, in vec3 center, in float radius, in float height
) {
    float w = sqrt(radius*radius - height*height);

    vec3 v_distance = center - point;
    vec2 q = vec2(length(v_distance.xz), v_distance.y);

    float s = max((height - radius)*q.x*q.x + w*w*(height + radius - 2.0*q.y),
                    height*q.x - w*q.y);

    return (s<0.0) ? length(q) - radius :
                        (q.x<w) ? height - q.y : length(q-vec2(w, height));
}

float sdDeathStar(in vec3 p2, in vec3 center,
                  in float ra, float rb,
                  in float d) {
    float a = (ra*ra - rb*rb + d*d) / (2.0*d);
    float b = sqrt(max(ra*ra-a*a, 0.0));

    vec3 v_distance = center - p2;
    vec2 p = vec2(v_distance.x, length(v_distance.yz));

    if (p.x*b - p.y*a > d*max(b-p.y, 0.0)) {
        return length(p-vec2(a, b));
    }
    else {
        return max((length(p)-ra), -(length(p-vec2(d, 0))-rb));
    }
}

float map(in vec3 pos) {
    float ra = 2.5;
    float rb = 1.0;

    return min(sdCutSphere(pos, vec3(5.0, 2.0, -3.0), ra, -1.00),
            sdDeathStar(pos, vec3(-3, -1, -3.5), ra, rb, 2));
}

float calcSoftShadow(in vec3 ro, in vec3 rd, float tmin, float tmax, const float k) {
    float res = 1.0;
    float t = tmin;
    for(int i=0; i<64; i++) {
        float h = map(ro + rd*t);
        res = min(res, k*h/t);
        t += clamp(h, 0.003, 0.10);
        if(res<0.002 || t>tmax) break;
    }
    return clamp(res, 0.0, 1.0);
}

vec3 calcNormal(in vec3 pos) {
    // Generic normal calculation
    vec2 e = vec2(1.0, -1.0)*0.5773;
    const float eps = 0.0005;
    return normalize(e.xyy*map(pos + e.xyy*eps) +
                     e.yyx*map(pos + e.yyx*eps) +
                     e.yxy*map(pos + e.yxy*eps) +
                     e.xxx*map(pos + e.xxx*eps));
}

vec3 ray_march(in vec3 ro, in vec3 rd, float tmax) {
    float t = 0.0;
    vec3 p = vec3(0.0, 0.0, 0.0);

    for (uint i=0; i<RAYMARCH_STEPS; i++) {
        p = ro + t*rd;
        float h = map(p);
        if (h<0.0001 || t>tmax) {
            break;
        }
        t += h;
    }

    vec3 col = vec3(0.0);
    if (t<tmax) {
        vec3 nor = calcNormal(p);
        vec3 lig = vec3(0.57703);
        float dif = clamp(dot(nor, lig), 0.0, 1.0);

        if (dif>0.001) {
            dif *= calcSoftShadow(p+nor*0.01, lig, 0.001, 1.0, 32.0);
        }
        float amb = 0.5 + 0.5*dot(nor, vec3(0.0, 1.0, 0.0));
        col = vec3(0.2, 0.3, 0.4) * amb + vec3(0.8, 0.7, 0.5)*dif;
    }

    return col;
}

void main() {
    float z_camera = -abs(cos(FOV/2.0) / sin(FOV/2.0));

    vec3 r_origin = ViewPos + vec3(0.0, 0.0, 1.0);
    vec3 r_direction = view_matrix * normalize(
        vec3(vs_in.FragPos.x, vs_in.FragPos.y / Aspect, z_camera)
    );

    FragColor = vec4(ray_march(r_origin, r_direction,  159.f), 1.0);
}
