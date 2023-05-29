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

const mat2 m2 = mat2( 0.60, -0.80, 0.80, 0.60);

vec3 Cloud(vec3 bgCol, vec3 ro, vec3 rd, vec3 cloudCol, float spd)
{
    vec3 col = bgCol;
    float t = sb.time * 0.15 * spd;
    vec3 sc = ro + rd * ((3.) * 40000.0 - ro.y) / rd.y;
    vec3 p = 0.0002 * sc;
    float f = 0.0;
    float s = 0.5;
    float sum = 0.;
    for(int i=0;i < 5; i++)
    {
        p += t;
        t *=1.5;
        f += s * SmoothNoise(p);
        sum += s;
        s *= 0.3;
    }

    float val = f/sum;
    col = mix( col, cloudCol, 0.5 * smoothstep(0.5, 0.8, val));
    return col;
}

vec3 RayMarchCloud(vec3 ro, vec3 rd)
{
    vec3 col = vec3(0.0, 0.0, 0.0);
    vec3 light_dir = sb.dir_light;
    light_dir.y *= -1;
    float sundot = clamp(dot(rd, normalize(light_dir)), 0.0, 1.0);

    col = vec3(0.2, 0.5, 0.85) * 1.1 + rd.y * rd.y * 0.5;
    col = mix(col, 0.85 * vec3(0.7, 0.75, 0.85), pow(1.0 - max(rd.y, 0.0), 4.0));

    col += 0.25 * vec3(1.0, 0.7, 0.4)*pow(sundot, 5.0);
    col += 0.25 * vec3(1.0, 0.8, 0.6)*pow(sundot, 64.0);
    col += 0.4 * vec3(1.0, 0.8, 0.6)*pow(sundot, 512.0);

    col = Cloud(col, ro, rd, vec3(1.0, 0.95, 1.0), 1.0);

    col = mix( col, 0.68 * vec3(0.4, 0.65, 1.0), pow(1.0 - max(rd.y, 0.0), 4.0));

    return col;
}

void main() {

    vec4 r_color = vec4(0, 0, 0, 1);

    vec3 cam_pos = vec3(0, 0, 0);
    vec2 uv = fragTexCoord - 0.5;

    mat3 look = look_at3(cam_pos, cam_pos + sb.cam_rot, vec3(0,1,0)) ;
    vec3 dir = normalize(look * uvToCastPlane(uv));
    dir.y *= -1;

    vec3 col = RayMarchCloud(cam_pos, dir);
    r_color.rgb = col;
    outColor = r_color;

}
