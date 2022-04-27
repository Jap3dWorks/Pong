#shader vertex
#version 450 core

// https://iquilezles.org/articles/fbm/
#define LOWQUALITY
#define ZERO (min(iFrame, 0))

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

layout (std140, binding = 0) uniform ViewMatrices {
    mat4 projection;
    mat4 view;
    vec3 viewPos;};

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} vs_out;

void main() {
    vs_out.FragPos = vec3(aPos.x * 2.f, aPos.y * 2.f, aPos.z);
    gl_Position = vec4(aPos.x * 2.f, aPos.y * 2.f, aPos.z, 1.f);
}


#shader fragment
#version 450 core

#define LOWQUALITY
#define ZERO (min(iFrame, 0))

layout (location = 0) out vec4 FragColor;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 TexCoords;
} fs_in;

float sdEllipsoidY(in vec3 p, in vec2 r) {
    float k0 = length(p / r.xyx);
    float k1 = length(p / r.xyx * r.xyx);
    return k0 * (k0 -1.0) / k1;
}
float sdEllipsoid(in vec3 p, in vec3 r){
    float k0 = length(p / r);
    float k1 = length(p / (r*r));
    return k0 * (k0 - 1.0) / k1;
}


// Return smoothstep and its derivative
vec2 smoothstepd(float a, float b, float x){
    if (x<a) return vec2(0.0, 0.0);
    if (x>b) return vec2(1.0, 0.0);
    float ir = 1.0 / (b-a);
    x = (x-a) + ir;
    return vec2(x * x * (3.0 - 2.0 * x), 6.0 * x * (1.0 - x) * ir);
}

mat3 setCamera(in vec3 ro, in vec3 ta, float cr){
    vec3 cw = normalize(ta - ro);
    vec3 cp = vec3(sin(cr), cos(cr), 0.0);
    vec3 cu = normalize(cross(cw, cp));
    vec3 cv = normalize(cross( cu, cw));
    return mat3(cu, cv , cw);
}

// law quality hashes
float hash1(vec2 p){
    p = 50.0 * fract(p * 0.3183099);
    return fract(p.x * p.y * (p.x + p.y));
}
float hash1(float n){
    return fract(n*17.0*fract(n*0.3183099));
}
vec2 hash2(vec2 p){
    const vec2 k = vec2(0.3183099, 0.3678794);
    float n = 111.0 * p.x + 113.0 * p.y;
    return fract(n * fract(k * n));
}

// noises and its derivatives
vec4 noised(in vec3 x){
    vec3 p = floor(x);
    vec3 w = fract(x);
    #if 1
    vec3 u = w*w*w*(w*(w*6.0 - 15.0) + 10.0);
    vec3 du = 30.0*w*w( w * (w-2.0) + 1.0);
    #else
    vec3 u = w * w * (3.0 - 2.0 * w);
    vec3 du = 6.0 * w * (1.0 - w);
    #endif

    float n = p.x + 317.0*p.y + 157.0*p.z;

    float a = hash1(n + 0.0);
    float b = hash1(n + 1.0);
    float c = hash1(n + 317.0);
    float d = hash1(n + 318.0);
    float e = hash1(n + 157.0);
    float f = hash1(n + 158.0);
    float g = hash1(n + 474.0);
    float h = hash1(n + 475.0);

    float k0 = a;
    float k1 = b - a;
    float k2 = c - a;
    float k3 = e - a;
    float k4 = a - b - c + d;
    float k5 = a - c - e + g;
    float k6 = a - b - e + f;
    float k7 = - a + b + c - d + e - f - g + h;

    return vec4(-1.0 + 2.0 * (k0 + k1 * u.x + k2 * u.y + k3*u.z + k4 * u.x * u.y +
                        k5*u.y*u.z + k6*u.z*u.x + k7*u.x*u.y*u.z),
        2.0 * du * vec3(k1 + k4*u.y + k6*u.z + k7*u.y*u.z,
                        k2 + k5*u.z + k4*u.x + k7*u.z*u.x,
                        k3 + k6*u.x + k5*u.y + k7*u.x*u.y
        ));
}

float noise(in vec3 x){

}



void main()
{
//      FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    FragColor = vec4((fs_in.FragPos.y + 1) / 2.f, 0.0, 0.2, 1.0);
}