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
#version 450

#define RAYMARCH_STEPS 256
#define HW_PERFORMANCE
#define SHADOW_RAY_MAX 80.f
#define MAX_SHADOW_STEPS 64
#define RAY_MAX 150.f
#define RAY_MIN 0.0001f

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

in CAM_DT {
    mat3 view_matrix;
    float z_camera_ray;
} cam_dt;

float fTime = float(Time);



// shapes
float sdCutSphere(
    in vec3 point, in float radius, in float height
) {
    float w = sqrt(radius*radius - height*height);

    vec3 v_distance = point;
    vec2 q = vec2(length(v_distance.xz), v_distance.y);

    float s = max((height - radius)*q.x*q.x + w*w*(height + radius - 2.0*q.y),
                    height*q.x - w*q.y);

    return (s<0.0) ? length(q) - radius :
                        (q.x<w) ? height - q.y : length(q-vec2(w, height));
}

float sdDeathStar(in vec3 point, in float ra,
                  float rb, in float d) {
    float a = (ra*ra - rb*rb + d*d) / (2.0*d);
    float b = sqrt(max(ra*ra-a*a, 0.0));

    vec3 v_distance = point;
    vec2 p = vec2(v_distance.x, length(v_distance.yz));

    if (p.x*b - p.y*a > d*max(b-p.y, 0.0)) {
        return length(p-vec2(a, b));
    }
    else {
        return max((length(p)-ra), -(length(p-vec2(d, 0))-rb));
    }
}

float sdCapsule(in vec3 p, in vec3 a, in vec3 b, in float r) {

    vec3 pa = p - a, ba = b - a;
    float h = clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0);
    return length(pa - ba * h) - r;
}

float checkersGrabBox(in vec2 p, in vec2 dpdx, in vec2 dpdy) {
    // filter kernel
    vec2 w = abs(dpdx) + abs(dpdy) + 0.001;
    // analytical integral
    // vec2 s = sign(fract(p*0.5) - 0.5);
    vec2 i = 2.0*(
    abs(fract((p - 0.5*w) * 0.5) - 0.5) - abs(fract((p + 0.5 * w) * 0.5) - 0.5)
                  ) / w;

    return 0.5 - 0.5 * i.x * i.y;
}

vec2 map(in vec3 pos) {
    // x=distance, y=(shape, material)
    vec2 res = vec2(0.0, -2.0);

    float ra = 2.5;
    float rb = 1.0;

    float d;
    vec2 temp = res;

    res = vec2(sdCutSphere(pos - vec3(5.0, 2.0, -3.0), ra, -1.0), 1.0);
    temp = vec2(sdDeathStar(pos - vec3(-3, -1, -3.5) , ra, rb, 2), 2.0);
    if (temp.x < res.x) {res = temp;}

    temp = vec2(sdCapsule(pos, vec3(-1.0, 3.5, -7.0), vec3(2.0, -3.5, -9.0), 2.0), 3.0);
    if (temp.x < res.x) {res = temp;}

    return res;
}

float calcSoftShadow(in vec3 ro, in vec3 rd, float tmin,
                    float tmax, const float k) {
    float res = 1.0;
    float t = tmin;
    for(int i=0; i<MAX_SHADOW_STEPS; i++) {
        float h = map(ro + rd*t).x;
        res = min(res, k*h/t);
        t += clamp(h, 0.003, 0.10);
        if(res<tmin || t>tmax) break;
    }
    return clamp(res, 0.0, 1.0);
}

vec3 calcNormal(in vec3 pos) {
    // Generic normal calculation
    vec2 e = vec2(1.0, -1.0)*0.5773;
    const float eps = 0.0005;
    return normalize(e.xyy*map(pos + e.xyy*eps).x +
                     e.yyx*map(pos + e.yyx*eps).x +
                     e.yxy*map(pos + e.yxy*eps).x +
                     e.xxx*map(pos + e.xxx*eps).x);
}

float calcAO(in vec3 pos, in vec3 nor) {
    float occ = 0.0;
    float sca = 1.0;
    for (int i=0; i<5; i++) {
        float h = 0.01 + 0.12 * float(i) / 0.4;
        float d = map(pos + h*nor).x;
        occ += (h-d)*sca;
        sca *= 0.95;
        if (occ > 0.35) break;
    }

    return clamp(1.0 - 3.0 * occ, 0.0, 1.0) * (0.5+0.5*nor.y);
}

vec2 raycast(in vec3 ro, in vec3 rd, float tmax, float tmin) {
    vec2 res = vec2(RAY_MAX, -2.0);

    // raytrace floor plane
    float tp1  = (0.0 - ro.y) / rd.y;
    if (tp1 > 0.0) {
//        tmax = min(tmax, tp1);
        res = vec2(tp1, 0.0);
    }

    // Raymarch geometries
    float t = 0.0;
    vec3 p = vec3(0.0, 0.0, 0.0);
    for (uint i=0; i<RAYMARCH_STEPS; i++) {
        p = ro + t*rd;
        vec2 h = map(p);
        if (abs(h.x)<tmin) {
            res = vec2(t, h.y);
            break;
        }
        else if (t>tmax) {
            break;
        }
        t += h.x;
    }

    return res;

}

vec3 render(in vec3 ro, in vec3 rd, in vec3 ddx_rd, in vec3 ddy_rd) {
    // background
    vec3 col = vec3(0.7, 0.7, 0.9) - max(rd.y, 0.0)*0.3;

    vec2 res = raycast(ro, rd, RAY_MAX, RAY_MIN);

    // hit something
    if (res.x < RAY_MAX) {
        vec3 pos = ro + rd*res.x;
        vec3 nor = (res.y==0) ? vec3(0.0, 1.0, 0.0) : calcNormal(pos);
        vec3 ref = reflect(rd, nor);

        vec3 lig = vec3(0.57703);  // TODO: lights from scene
        float dif = clamp(dot(nor, lig), 0.0, 1.0);

        col = sin(vec3(0.2, 0.5, .9)*res.y);
        if (res.y==0) {
            // floor
            vec3 dpdx = ro.y * (rd/rd.y - ddx_rd/ddx_rd.y);
            vec3 dpdy = ro.y * (rd/rd.y + ddy_rd/ddy_rd.y);

            float f = checkersGrabBox(3.0*pos.xz, 3.0*dpdx.xz, 3.0*dpdy.xz);
            col = 0.15 + f * vec3(0.05);
//            ks = 0.4;
        }

        if (dif>0.001) {
            dif *= calcSoftShadow(
                ro + rd*res.x + nor*0.01, lig, 0.01, SHADOW_RAY_MAX, 32.0
            );
        }

        float amb = 0.5 + 0.5*dot(nor, vec3(0.0, 1.0, 0.0));
        col = col + amb + vec3(0.8, 0.7, 0.5)*dif;
    }

    return clamp(col, 0.0, 1.0);
}

void main() {
    // TODO: paint floor and sky
    // https://www.shadertoy.com/view/Xds3zN

    vec3 r_origin = ViewPos + vec3(0.0, 0.0, 1.0);

    float y_incr = 2 / (RenderHeight * Aspect);

    // filtering rays
    vec3 ddx_rd = cam_dt.view_matrix * normalize(
        vec3(vs_in.FragPos.x + 2 / RenderWidth,
            vs_in.FragPos.y / Aspect,
            cam_dt.z_camera_ray
        ));

    vec3 ddy_rd = cam_dt.view_matrix * normalize(
        vec3(vs_in.FragPos.x,
        (vs_in.FragPos.y + 2 / RenderHeight) / Aspect,
        cam_dt.z_camera_ray
        ));

    // Camera Rays
    vec3 r_direction = cam_dt.view_matrix * normalize(
        vec3(vs_in.FragPos.x,
        vs_in.FragPos.y / Aspect,
        cam_dt.z_camera_ray)
    );

    FragColor = vec4(render(r_origin, r_direction, ddx_rd, ddy_rd), 1.0);
}
