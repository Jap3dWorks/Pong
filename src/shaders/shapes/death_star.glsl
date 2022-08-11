#ifndef DEATH_STAR_GLSL
#define DEATH_STAR_GLSL

float sd_death_star(in vec3 point, in float ra,
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

#endif // DEATH_STAR_GLSL