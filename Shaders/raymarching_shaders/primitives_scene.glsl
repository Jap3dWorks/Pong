#shader vertex

void main() {
    cam_dt.z_camera_ray = -abs(cos(FOV/2.0) / sin(FOV/2.0));
    vs_out.FragPos = vec3(aPos.x, aPos.y, aPos.z);
    cam_dt.view_matrix = inverse(mat3(View));
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.f);
}


#shader fragment

#define PI 3.141592653589793f

#define HW_PERFORMANCE

#define RAYMARCH_STEPS 256
#define RAY_MAX 150.f
#define RAY_MIN 0.001f
#define STEP_FACTOR 0.75

#define MAX_SHADOW_STEPS 64
#define SHADOW_RAY_MAX 80.f
#define SHADOW_BIAS 0.09f
#define SHADOW_RAY_MIN 0.001
#define SHADOW_PENUMBRA_FACTOR 16.0

#define FILTERING_KERNEL_SIZE_FACTOR 0.0009

#define SUN_LIGHT
#define SKY_LIGHT
#define BACK_LIGHT
#define SSS_LIGHT
#define FOG

#ifdef HW_PERFORMANCE
#define AA 1
#else
#define AA 3
#endif

float fTime = float(Time);


#include "operations/quaternion.glsl"
#include "operations/boolean.glsl"
#include "operations/transform.glsl"
#include "shapes/cut_sphere.glsl"
#include "shapes/death_star.glsl"
#include "shapes/capsule.glsl"
#include "shapes/octahedron.glsl"
#include "shapes/plane.glsl"
#include "shapes/torus.glsl"
#include "textures/checker.glsl"
#include "render/render.glsl"
#include "operations/round.glsl"
#include "deform/displacement.glsl"
#include "shapes/sphere.glsl"

// Map
// ---
vec2 map(in vec3 pos) {
    // x=distance, y=(shape, material)
    vec2 res = vec2(0.0, -2.0);

    float ra = 2.5;
    float rb = 1.0;

    float d;
    vec2 temp = res;

    res = vec2(sd_cut_sphere(
        op_transform(pos - vec3(6.0, 5.0, -3.0), vec3(1.0, 0.0, 0.0), PI/2.0), ra, -1.0),
        1.0);
    res.x = op_round(res.x, .05);
    temp = vec2(sd_death_star(pos - vec3(-6.0, 5.0, -3.0) , ra, rb, 2), 2.0);
    temp.x = op_round(temp.x, 0.025);
    if (temp.x < res.x) {res = temp;}

    temp = vec2(sd_capsule(pos, vec3(-1.0, 6.5, -7.0), vec3(2.0, 0.5, -9.0), 2.0), 3.0);
    if (temp.x < res.x) {res = temp;}

    temp = vec2(sd_octahedron(
        op_transform(pos - vec3(0.0, 15.0 + sin(float(Time)) * 0.75, -8.0),
        vec3(0.0, 1.0, 0.0),
        float(Time) * 0.2), 6.0), 4.0);
    temp.x = op_round(temp.x, .25);
    if (temp.x < res.x) {res = temp;}

    // --------------------
    // temp = vec2(sd, 5.0);
    // temp = vec2(sd_torus(pos, 1.0, 0.2), 5.0);
    temp = vec2(
        op_smooth_union(
            sd_torus(op_transform(pos - vec3(-1.0, 3.0, -4.0), vec3(1.0, 0.0, 0.0), float(Time*0.5)), 1.0, 0.2),
            sd_torus(op_transform(pos - vec3(0.5+sin(float(Time)), 3.0, -4.0), vec3(1.0, 0.0, 0.0), PI*0.5+float(Time*0.5)), 1.0, 0.2),
            (1.05 + sin(float(Time) * 0.21)) * 1.2)
        , 5.0);
    if(temp.x < res.x) {res = temp;}

    temp = vec2(
        op_sin_displace(
            sd_sphere(pos - vec3(sin(fTime*0.2)*5.0, 0.0, 0.0), 2.0),
            pos, vec3(1.0), vec3(10.0), 0.15), 6.0);

    if (temp.x < res.x) {res=temp;}

    return res;
}

vec2 raycast(in vec3 ro, in vec3 rd, float tmax, float tmin) {
    vec2 res = vec2(RAY_MAX, -2.0);

    // raytrace floor plane
    float tp1 = rt_plane(ro, rd, vec3(0, -0.01, 0), vec3(0,1,0));

    if (tp1 < RAY_MAX) {
        res = vec2(tp1, 0.0);
    }

    // Raymarch geometries
    float t = 0.0;
    vec3 p = vec3(0.0, 0.0, 0.0);
    for (uint i=0; i<RAYMARCH_STEPS; i++) {
        p = ro + t*rd;
        vec2 h = map(p);
        if (h.x < tmin) {
            res = (res.x < t) ? res : vec2(t, h.y);
            break;
        }
        else if (t>tmax) {
            break;
        }
//        t += h.x * clamp(STEP_FACTOR * h.x, STEP_FACTOR, 1.0);
//        t += h.x * clamp(0.95 * i, STEP_FACTOR, 1.0);
        #if STEP_FACTOR == 1
        t += h.x;
        #else
        t += h.x * STEP_FACTOR;
        #endif

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
        vec3 nor = (res.y==0.0) ? vec3(0.0, 1.0, 0.0) : calc_normal(pos);
        vec3 ref = reflect(rd, nor);

        col = 0.2 + 0.2 * sin(res.y*2.0 + vec3(0.0, 1.0, 2.0));
        float ks = 0.85;

        if (res.y==0.0) {
            // floor
            // Filtering footprints increase its sizes in
            // relation of distance (res.x) and camera aperture z_camera_ray
            vec3 dpdx = res.x * cam_dt.z_camera_ray * (rd/rd.y - ddx_rd/ddx_rd.y) * FILTERING_KERNEL_SIZE_FACTOR;
            vec3 dpdy = res.x * cam_dt.z_camera_ray * (rd/rd.y + ddy_rd/ddy_rd.y) * FILTERING_KERNEL_SIZE_FACTOR;

            float f = checkers_grab_box(pos.xz,
                        dpdx.xz,
                        dpdy.xz);

            col = 0.15 + f * vec3(0.05);
            ks = 0.4;
        }

        // lighting
        float occ = calc_ao(pos, nor);
        vec3 lin = vec3(0.0);

        // sun
        #ifdef SUN_LIGHT
        {
            // TODO: lights from scene
            vec3 lig = normalize(vec3(0.5, 0.4, 0.6));
            vec3 hal = normalize(lig-rd);
            float dif = clamp(dot(nor, lig), 0.0, 1.0);

            dif *= (occ*0.65 + 0.35);
            dif *= calc_soft_shadow(pos, lig, SHADOW_RAY_MIN, SHADOW_RAY_MAX);
            float spe = pow(clamp(dot(nor, hal), 0.0, 1.0), 36);  // sun specular

            spe *= dif;
            spe *= 0.04 + 0.96*pow(clamp(1.0-dot(hal, lig), 0.0, 1.0), 5.0);
            lin += col * 2.20 * dif * vec3(1.30, 1.0, 0.7);
            lin += 5.00 * spe * vec3(1.3, 1.0, 0.7) * ks;
        }
        #endif

        // sky
        #ifdef SKY_LIGHT
        {
            float dif  = sqrt(clamp(0.5 + 0.5 * nor.y, 0.0, 1.0));
            dif *= occ;
            float spe  = smoothstep(-0.2, 0.2, ref.y);
            spe *= dif;
            spe *= 0.04 + 0.96 * pow (clamp(1.0 + dot(nor, rd), 0.0, 1.0), 5.0);
            spe *= calc_soft_shadow(pos, ref, SHADOW_RAY_MIN, SHADOW_RAY_MAX);  // oclude other geometries
            lin += col * 0.06 * dif * vec3(0.4, 0.6, 1.15);
            lin += 1.0 * spe * vec3(0.40, 0.60, 1.3) * ks;
        }
        #endif

        // Back
        #ifdef BACK_LIGHT
        {
            float dif = clamp(dot(nor, normalize(vec3(0.5, 0.0, 0.6))), 0.0, 1.0) *
                clamp(1.0 - pos.y, 0.0, 1.0);
//            dif *= occ;
            lin += col * 0.55 * dif * vec3(0.25, 0.25, 0.25);
        }
        #endif

        // sss
        #ifdef SSS_LIGHT
        {
            float dif = pow(clamp(1.0 + dot(nor, rd), 0.0, 1.0), 2.0);
//            dif *= occ;
            lin += col * 0.25 * dif * vec3(1.0, 1.0, 1.0);
        }
        #endif

        col = lin;

        // fog
        #ifdef FOG
        col = mix(col, vec3(0.7, 0.7, 0.9), 1.0 - (exp(-0.000001*res.x*res.x*res.x)));
        #endif
    }

    return clamp(col, 0.0, 1.0);
}

// Main
// ----
void main() {
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
        ));

    // Camera Rays
    vec3 r_direction = cam_dt.view_matrix * normalize(
        vec3(aspect_coords,
        cam_dt.z_camera_ray));

    vec3 col = render(r_origin, r_direction, rdx, rdy);

    // gamma
    col = pow(col, vec3(0.4545));

    FragColor = vec4(col, 1.0);
}
