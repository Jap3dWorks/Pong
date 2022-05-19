#ifndef RENDER_GLSL
#define RENDER_GLSL

#include "render/map.glsl"

float calc_soft_shadow(in vec3 ro, in vec3 rd, in float tmin, in float tmax) {
    float res = 1.f;
    float ph = 1e20;

    for (float t=tmin + SHADOW_BIAS; t<tmax;) {
        float h = map(ro + rd*t).x;
        if (h.x < tmin){
            return 0.0;
        }

        float y = h * h / (2.0*ph);
        float d = sqrt(h*h - y*y);
        res = min(res, SHADOW_PENUMBRA_FACTOR * d/max(0.0, t-y));
        ph = h;
        t += h;
    }

    return res;
    //    return clamp(res, 0.0, 1.0);
}

vec3 calc_normal(in vec3 pos) {
    // Generic normal calculation
    vec2 e = vec2(1.0, -1.0)*0.5773;
    const float eps = 0.0005;
    return normalize(e.xyy*map(pos + e.xyy*eps).x +
    e.yyx*map(pos + e.yyx*eps).x +
    e.yxy*map(pos + e.yxy*eps).x +
    e.xxx*map(pos + e.xxx*eps).x);
}

/*0 -> AO, 1 -> No AO*/
float calc_ao(in vec3 pos, in vec3 nor) {
    float occ = 0.0;
    float sca = 1.0;

    for (int i=0; i<5; i++) {
        float h = 0.05 + 0.45 * float(i);
        float d = map(pos + h*nor).x;
        occ += (h-d) * sca;
        sca *= 0.20;
        if (occ > 0.35) break;
    }
    return clamp(1.0 - occ * 3.0, 0.0, 1.0);
}


#endif // RENDER_GLSL
