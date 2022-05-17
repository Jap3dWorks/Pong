#ifndef VORONOI_GLSL
#define VORONOI_GLSL

float voronoi(in vec2 x) {
    ivec2 p = floor(x);
    vec2 f = fract(x);

    float res = 8.0;
    for(int j=-1; j<=1; j++) {
        for (int i=-1; i<=1; i++) {
            ivec2 b = ivec2(i, j);
            vec2 r = vec2(b) - f + hash2f(p + b);
            float d = dot(r, r);
            res = min(res, d);
        }
    }
    return sqrt(res);
}

#endif // VORONOI_GLSL
