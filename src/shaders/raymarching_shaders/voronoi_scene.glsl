#shader vertex

void vertex_main() {
    cam_dt.z_camera_ray = -abs(cos(FOV/2.0) / sin(FOV/2.0));
    vs_out.FragPos = vec3(aPos.x, aPos.y, aPos.z);
    cam_dt.view_matrix = inverse(mat3(View));
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.f);
}


#shader fragment
#define PI 3.141592653589793f

float fTime = float(Time);

#include "textures/voronoi.glsl"

void main() {
    vec2 aspect_coords = vec2(vs_in.FragPos.x, vs_in.FragPos.y / Aspect);
    vec4 v = smooth_voronoi(aspect_coords * 6.0, 0.05);

    // gamma
    vec3 col = sqrt(v.yzw);
    FragColor = vec4(col, 1.0);
}
