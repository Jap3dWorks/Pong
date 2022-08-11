#ifndef QUATERNION_GLSL
#define QUATERNION_GLSL

vec4 inverse(in vec4 quaternion) {
    return vec4(-quaternion.xyz, quaternion.w) / sqrt(quaternion.x*quaternion.x +
    quaternion.y*quaternion.y +
    quaternion.z*quaternion.z +
    quaternion.w*quaternion.w);;
}

vec4 quat_from_axis_angle(in vec3 axis, in float radians) {
    vec4 qr;
    float half_angle = radians * 0.5;
    qr.x = axis.x * sin(half_angle);
    qr.y = axis.y * sin(half_angle);
    qr.z = axis.z * sin(half_angle);
    qr.w = cos(half_angle);
    return qr;
}


#endif // QUATERNION_GLSL
