#ifndef PLANE_GLSL
#define PLANE_GLSL

float rt_plane(in vec3 ro, in vec3 rd, in vec3 po, in vec3 pn) {
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

#endif // PLANE_GLSL

