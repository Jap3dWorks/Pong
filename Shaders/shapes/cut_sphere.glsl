#ifndef CUT_SPHERE_GLSL
#define CUT_SPHERE_GLSL

float sd_cut_sphere(in vec3 point, in float radius, in float height) {
    float w = sqrt(radius*radius - height*height);

    vec3 v_distance = point;
    vec2 q = vec2(length(v_distance.xz), v_distance.y);

    float s = max((height - radius)*q.x*q.x + w*w*(height + radius - 2.0*q.y),
    height*q.x - w*q.y);

    return (s<0.0) ? length(q) - radius :
    (q.x<w) ? height - q.y : length(q-vec2(w, height));
}

#endif // CUT_SPHERE_GLSL
