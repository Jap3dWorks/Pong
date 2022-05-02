#shader vertex
#version 450

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

layout (std140, binding=0) uniform ViewMatrices {
    mat4 Projection;
    mat4 View;
    vec3 ViewPos;};

layout (std140, binding=2) uniform FrameData {
    double DeltaTime;
    double Time;
    uint Frame;
    float Fps;
};

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

void main() {
    vs_out.FragPos = vec3(aPos.x, aPos.y, aPos.z);
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.f);
}


#shader fragment
#version 450

layout (std140, binding=0) uniform ViewMatrices {
    mat4 Projection;
    mat4 View;
    vec3 ViewPos;};

layout (std140, binding=3) uniform RenderData {
    uint RenderWidth;
    uint RenderHeight;
    float ZNear;
    float ZFat;
};

layout (location=0) out vec4 FragColor;

in VS_OUT{
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
}vs_in;


float distance_from_sphere(in vec3 point, in vec3 center, float radius) {
    return length(point - center) - radius;
}

float map_the_world(in vec3 point) {
    float sphere_0 = distance_from_sphere(point, vec3(0.0), 1.0);
    return sphere_0;
}

vec3 calculate_normal(in vec3 p) {
    const vec3 small_step = vec3(0.001, 0.0, 0.0);
    float gradient_x =
        map_the_world(p + small_step.xyy) - map_the_world(p - small_step.xyy);
    float gradient_y =
        map_the_world(p + small_step.yxy) - map_the_world(p - small_step.yxy);
    float gradient_z =
        map_the_world(p + small_step.yyx) - map_the_world(p - small_step.yyx);

    vec3 normal = vec3(gradient_x, gradient_y, gradient_z);

    return normalize(normal);
}

vec3 ray_march(in vec3 r_origin, in vec3 r_direction) {
    float total_distance_traveled = 0.0;
    const int NUMBER_OF_STEPS = 32;
    const float MINIMUM_HIT_DISTANCE = 0.001;
    const float MAXIMUM_TRACE_DISTANCE = 1000.0;

    for (int i = 0; i<NUMBER_OF_STEPS; ++i) {
        vec3 current_position = r_origin + total_distance_traveled * r_direction;
        float distance_to_closest = map_the_world(current_position);

         if (distance_to_closest < MINIMUM_HIT_DISTANCE) {
             vec3 normal = calculate_normal(current_position);

//             return vec3(1.0, 1.0, 1.0);
             return normal * 0.5 + 0.5;
         }

        if (total_distance_traveled > MAXIMUM_TRACE_DISTANCE) {
            break;
        }

        total_distance_traveled += distance_to_closest;
    }

    return vec3(0.0);
}

void main() {
//    vec2 uv = vUV.st * 2.0 - 1.0;

    vec3 camera_position = vec3(ViewPos.xy, ViewPos.z);

    vec3 r_origin = camera_position;
//    vec3 r_origin = vec3(0.0, 0.0, -5.0);
    float aspect = float(RenderHeight) / float(RenderWidth);

    vec3 r_direction = vec3(vs_in.FragPos.x,
                            vs_in.FragPos.y * aspect,
                            1.0);

//    vec3 r_origin =
//    vec4 r_direction = View * vec4(vs_in.FragPos.x,
//                                vs_in.FragPos.y * aspect,
//                                1.0, 1.0);

//    vec3 r_direction = vs_in.FragPos;

    vec3 shader_color = ray_march(r_origin, r_direction.xyz);

    FragColor = vec4(shader_color, 1.0);


//    FragColor = vec4((vs_in.FragPos + 1.0) / 2.0, 1.0);
//      FragColor = vec4((vs_in.FragPos + 1.0) / 2.0, 1.0);
//    FragColor = vec4(ViewPos, 1.0);

}
