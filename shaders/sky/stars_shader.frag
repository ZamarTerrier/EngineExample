#version 450

#define PI 3.1415926536


layout(binding = 0) uniform SkyBuffer {
    float time;
    vec3 cam_pos;
    vec3 cam_rot;
    vec3 dir_light;
} sb;

layout(location = 0) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

mat3 look_at3(vec3 from, vec3 to, vec3 up){

    vec3 z = normalize(to - from);
    vec3 x = normalize(cross(up, z));
    vec3 y = normalize(cross(z, x));

    mat3 result = mat3(
        vec3(x.x, x.y, x.z),
        vec3(y.x, y.y, y.z),
        vec3(z.x, z.y, z.z));

    return result;
}

vec3 uvToCastPlane(vec2 uv){
    return vec3(uv.x, uv.y, 1.1/*CAM FOV*/);
}

float hash(const in float n){
    return fract(cos(n) * 4378.5453);
}

float pnoise(in vec3 o){
    vec3 p = floor(o);
    vec3 fr = fract(o);

    float n = p.x + p.y * 57.0 + p.z * 1009.0;

    float a = hash(n + 0.0);
    float b = hash(n + 1.0);
    float c = hash(n + 57.0);
    float d = hash(n + 58.0);

    float e = hash(n + 0.0 + 1009.0);
    float f = hash(n + 1.0 + 1009.0);
    float g = hash(n + 57.0 + 1009.0);
    float h = hash(n + 58.0 + 1009.0);

    vec3 fr2 = fr * fr;
    vec3 fr3 = fr2 * fr;

    vec3 t = 3.0 * fr2 - 2.0 * fr3;

    float u = t.x;
    float v = t.y;
    float w = t.z;

    float res1 = a + (b - a) * u + (c - a) * v + (a - b + d - c) * u * v;
    float res2 = e + (f - e) * u + (g - e) * v + (e - f + h - g) * u * v;

    float res = res1 * (1.0 - w) + res2 * (w);

    return res;
}

const mat3 m = mat3(0.00, 0.80, 0.60,
                    -0.80, 0.36, -0.48,
                    -0.60, -0.48, 0.66);

float SmoothNoise(vec3 p)
{
    float f;
    f = 0.5 * pnoise(p);
    p = m * p * 2.02;
    f += 0.25 * pnoise(p);

    return f * (1.0 / (0.5 + 0.25));
}

vec3 getNebula(in vec3 from, in vec3 dir, float level, float power)
{
    vec3 color = vec3(0.0);
    float nebula = pow(SmoothNoise(dir + 3.0), 12.0);

    if(nebula > 0)
    {
        vec3 pos = (dir.xyz + dir.xzy + dir.zyx) / 3.0;
        vec3 randc = vec3(SmoothNoise(dir.xyz * 10. * level));
        color = nebula * randc;
    }

    return pow(color * 2.25, vec3(power));
}

vec3 getStars(in vec3 from, in vec3 dir, float power)
{
    vec3 color = vec3(pow(SmoothNoise(dir * 400.0), 32.0));
    return pow(color * 2.25, vec3(power));
}

void main() {

    vec4 r_color = vec4(0, 0, 0, 1);

    vec3 cam_pos = vec3(0, 0, 0);
    vec2 uv = fragTexCoord - 0.5;

    mat3 look = look_at3(cam_pos, cam_pos + sb.cam_rot, vec3(0,1,0)) ;
    vec3 dir = normalize(look * uvToCastPlane(uv));
    dir.y *= -1;

    vec3 color = clamp(getNebula(cam_pos, dir, 1.0, 0.5) * 1.5, 0.0, 1.0) * vec3(1.0, 0.0, 0.0);

    vec3 starColor = clamp(getStars(cam_pos, dir, 0.9), 0.0, 1.0);

    vec3 col = clamp(color + starColor, 0.0, 1.0);
    r_color.rgb = col;
    outColor = r_color;

}
