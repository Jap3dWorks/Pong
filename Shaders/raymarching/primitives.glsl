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

#define HW_PERFORMANCE
#define RAYMARCH_STEPS 256
#define SHADOW_RAY_MAX 80.f
#define MAX_SHADOW_STEPS 64
#define RAY_MAX 150.f
#define RAY_MIN 0.0001f
#define FILTERING_KERNEL_SIZE_FACTOR 0.0009

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
//    float col_val = pow(mod(floor(p.x), 2) - mod(floor(p.y), 2), 2);
//    return col_val;

    // filter kernel
    vec2 w = abs(dpdx) + abs(dpdy) + 0.001;
    // analytical integral (box_filter)
    vec2 i = 2.0*(
    // xor pattern
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

    res = vec2(sdCutSphere(pos - vec3(5.0, 5.0, -3.0), ra, -1.0), 1.0);
    temp = vec2(sdDeathStar(pos - vec3(-3, 3, -3.5) , ra, rb, 2), 2.0);
    if (temp.x < res.x) {res = temp;}

    temp = vec2(sdCapsule(pos, vec3(-1.0, 6.5, -7.0), vec3(2.0, 0.5, -9.0), 2.0), 3.0);
    if (temp.x < res.x) {res = temp;}

    return res;
}

float calc_soft_shadow(in vec3 ro, in vec3 rd, float tmin, float tmax) {
    float res = 1.0;
    float t = tmin;

    for(int i=0; i<MAX_SHADOW_STEPS; i++) {
        float h = map(ro + rd*t).x;
        float s = clamp(8.0*h/t, 0.0, 1.0);
        res = min(res, s*s*(3.0-2.0*s));

        t += clamp(h, 0.02, 0.2);
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

float rtPlane(in vec3 ro, in vec3 rd, in vec3 po, in vec3 pn) {
    vec3 pnn = normalize(pn);
    vec3 rdn = normalize(rd);

    // dot(po-ro, pn) / dot(rd, pn) = t
    // dot(rd, pn) == 0 no colisiona
    // t < 0 no colisiona

    float d = dot(rd, pn);
    if (d == 0) {
        return RAY_MAX;
    }

    float t = dot(po-ro, pn) / d;

    return (t>=0) ? t : RAY_MAX;
}

vec2 raycast(in vec3 ro, in vec3 rd, float tmax, float tmin) {
    vec2 res = vec2(RAY_MAX, -2.0);

    // raytrace floor plane
    float tp1 = rtPlane(ro, rd, vec3(0, -0.01, 0), vec3(0,1,0));

    if (tp1 < RAY_MAX) {
        res = vec2(tp1, 0.0);
    }

//    float tp1  = (0.0 - ro.y) / rd.y;
//    if (tp1 > 0.0) {
//        tmax = min(tmax, tp1);
//        res = vec2(tp1, 0.0);
//    }

    // Raymarch geometries
    float t = 0.0;
    vec3 p = vec3(0.0, 0.0, 0.0);
    for (uint i=0; i<RAYMARCH_STEPS; i++) {
        p = ro + t*rd;
        vec2 h = map(p);
        if (abs(h.x)<tmin) {
            res = (res.x < t) ? res : vec2(t, h.y);
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
        vec3 nor = (res.y==0.0) ? vec3(0.0, 1.0, 0.0) : calcNormal(pos);
        vec3 ref = reflect(rd, nor);

//        vec3 lig = vec3(0.57703);  // TODO: lights from scene
//        float dif = clamp(dot(nor, lig), 0.0, 1.0);

//        col = sin(vec3(0.1, 0.4, .8)*res.y);
        col = 0.2 + 0.2 * sin(res.y*2.0 + vec3(0.0, 1.0, 2.0));
        float ks = 1.0;

        if (res.y==0.0) {
            // floor
            // Filtering footprints increase its sizes in
            // relation of distance (res.x) and camera aperture z_camera_ray
            vec3 dpdx = res.x * cam_dt.z_camera_ray * (rd/rd.y - ddx_rd/ddx_rd.y) * FILTERING_KERNEL_SIZE_FACTOR;
            vec3 dpdy = res.x * cam_dt.z_camera_ray * (rd/rd.y + ddy_rd/ddy_rd.y) * FILTERING_KERNEL_SIZE_FACTOR;

//            float f = checkersGrabBox(pos.xz / 2.0, dpdx.xz / 2.0, dpdy.xz/2.0);
            float f = checkersGrabBox(pos.xz,
                        dpdx.xz,
                        dpdy.xz);

            col = 0.15 + f * vec3(0.05);
        }

        // lighting
        float occ = calcAO(pos, nor);
        vec3 lin = vec3(0.0);

        // sun
        {
            vec3 lig = normalize(vec3(0.5, 0.4, 0.6));
            vec3 hal = normalize(lig-rd);
            float dif = clamp(dot(nor, lig), 0.0, 1.0);

            dif *= calc_soft_shadow(pos, lig, 0.02, SHADOW_RAY_MAX);

//            float spe = pow(clamp(dot(nor, hal), 0.0, 1.0), 16);  // sun specular
//            spe *= dif;
//            spe *= 0.04 + 0.96*pow(clamp(1.0-dot(hal, lig), 0.0, 1.0), 5.0);
            lin += col * 2.20 * dif * vec3(1.30, 1.0, 0.7);
//            lin += 5.00 * spe * vec3(1.3, 1.0, 0.7) * ks;
        }

        // sky
//        {
//            float dif  = sqrt(clamp(0.5 + 0.5 * nor.y, 0.0, 1.0));
//            dif *= occ;
//            float spe  = smoothstep(-0.2, 0.2, ref.y);
//            spe *= dif;
//            spe *= 0.04 + 0.96 * pow (clamp(1.0 + dot(nor, rd), 0.0, 1.0), 5.0);
//            spe *= calc_soft_shadow(pos, ref, 0.02, SHADOW_RAY_MAX);  // oclude other geometries
//            lin += col * 0.06 * dif * vec3(0.4, 0.6, 1.15);
//            lin += 2.0 * spe * vec3(0.40, 0.60, 1.3) * ks;
//        }

        // Back
//        {
//            float dif = clamp(dot(nor, normalize(vec3(0.5, 0.0, 0.6))), 0.0, 1.0) *
//                clamp(1.0 - pos.y, 0.0, 1.0);
//            dif *= occ;
//            lin += col * 0.55 * dif * vec3(0.25, 0.25, 0.25);
//        }

        // sss
//        {
//            float dif = pow(clamp(1.0 + dot(nor, rd), 0.0, 1.0), 2.0);
//            dif *= occ;
//            lin += col * 0.25 * dif * vec3(1.0, 1.0, 1.0);
//        }

        col = lin;
        // fog
        col = mix(col, vec3(0.7, 0.7, 0.9), 1.0 - (exp(-0.000001*res.x*res.x*res.x)));

    }

    return clamp(col, 0.0, 1.0);
}

void main() {
    // TODO: paint floor and sky
    // https://www.shadertoy.com/view/Xds3zN

    vec3 r_origin = ViewPos + vec3(0.0, 0.0, 1.0);

    float y_incr = 2 / (RenderHeight * Aspect);
    vec2 aspect_coords = vec2(vs_in.FragPos.x, vs_in.FragPos.y / Aspect);
    float diff_increment = 2 / RenderWidth;

    // filtering rays
    vec3 rdx = cam_dt.view_matrix * normalize(
        vec3(diff_increment + aspect_coords.x,
            vs_in.FragPos.y / Aspect,
            cam_dt.z_camera_ray
        ));

    vec3 rdy = cam_dt.view_matrix * normalize(
        vec3(aspect_coords.x,
        diff_increment + aspect_coords.y,
        cam_dt.z_camera_ray
        )
    );

    // Camera Rays
    vec3 r_direction = cam_dt.view_matrix * normalize(
        vec3(aspect_coords,
        cam_dt.z_camera_ray)
    );

    vec3 col = render(r_origin, r_direction, rdx, rdy);

    // gamma
    col = pow(col, vec3(0.4545));

    FragColor = vec4(col, 1.0);
}
