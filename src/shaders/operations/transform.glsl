#ifndef TRANSFORM_GLSL
#define TRANSFORM_GLSL

#include "operations/quaternion.glsl"

vec3 op_transform(in vec3 point, in vec4 transform) {
    return point + 2.0 * cross(transform.xyz,
        cross(transform.xyz, point) + transform.w * point);
}

vec3 op_transform(in vec3 point, in vec3 transform) {
    return point - transform;
}

vec3 op_transform(in vec3 point, in mat3 transform) {
    return transform * point;
}

vec3 op_transform(in vec3 point, in mat4 transform) {
    return (transform * vec4(point, 1.0)).xyz;
}

vec3 op_transform(in vec3 point, in vec3 axis, in float angle) {
    vec4 q = quat_from_axis_angle(axis, angle);
    return op_transform(point, q);
}

#endif // TRANSFORM_GLSL
