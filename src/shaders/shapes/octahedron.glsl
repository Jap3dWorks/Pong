#ifndef OCTAHEDRON_GLSL
#define OCTAHEDRON_GLSL

float sd_octahedron(in vec3 point, in float size) {
    point = abs(point);
    float m = point.x + point.y + + point.z - size;
    vec3 q;

    if (3.0 * point.x < m) q=point.xyz;
    else if (3.0 * point.y < m) q=point.yzx;
    else if (3.0 * point.z < m) q=point.zxy;

    else return m * 0.57735027;

    float k = clamp(0.5*(q.z - q.y + size), 0.0, size);
    return length(vec3(q.x, q.y-size+k, q.z-k));
}

#endif // OCTAHEDRON_GLSL
