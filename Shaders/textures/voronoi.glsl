#ifndef VORONOI_GLSL
#define VORONOI_GLSL

float _voronoi_hash1(float n) {
    return fract(sin(n) * 43758.5453);
}

vec2 _voronoi_hash2(in vec2 p) {
    p=vec2(dot(p, vec2(127.1, 311.7)), dot(p, vec2(269.5, 183.3)));
    return fract(sin(p)) * 43758.5453;
}

vec2 _voronoi_hash2(in ivec2 p) {
    return _voronoi_hash2(vec2(float(p.x), float(p.y)));

}


//float voronoi(in vec2 x) {
//    ivec2 p = floor(x);
//    vec2 f = fract(x);
//
//    float res = 8.0;
//    for(int j=-1; j<=1; j++) {
//        for (int i=-1; i<=1; i++) {
//            ivec2 b = ivec2(i, j);
//            vec2 r = vec2(float(b.x), float(b.y)) - f + _voronoi_hash2(p + b);
//            float d = dot(r, r);
//            res = min(res, d);
//        }
//    }
//
//    return sqrt(res);
//}

/*
* #param: w: smoothness*/
vec4 smooth_voronoi(in vec2 point, in float w, in float evolution) {
    vec2 n = floor(point);
    vec2 f = fract(point);

    vec4 m = vec4(8.0, 0.0, 0.0, 0.0);
    for(int j=-2; j<=2; j++) {
        for(int i=-2; i<=2; i++) {
            vec2 g = vec2(float(i), float(j));
            vec2 o = _voronoi_hash2(n + g);

            // animate
            o = 0.5 + 0.5*sin(evolution + o);

            // distance to cell
            float d = length(g - f + o);

            // cell color
            vec3 col = vec3( _voronoi_hash1(dot(n + g, vec2(7.0, 113.0))));
//            vec3 col = 0.5 + 0.5*sin(_voronoi_hash1(dot(n+g, vec2(7.0, 113.0)))*2.5 + 3.5 + vec3(2.0, 3.0, 0.0));
//            col = col * col;

            // smooth
            float h = smoothstep(-1.0, 1.0, (m.x - d) / w);
            m.x = mix(m.x, d, h) - h*(1.0-h)*w/(1.0+3.0*w);
            m.yzw = mix(m.yzw, col, h) - h*(1.0-h)*w/(1.0+3.0*w);
        }
    }
    return m;
}

vec4 smooth_voronoi(in vec2 point, in float w) {
    return smooth_voronoi(point, w, 1.0);
}

#endif // VORONOI_GLSL
