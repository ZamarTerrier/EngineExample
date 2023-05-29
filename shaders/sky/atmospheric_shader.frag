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

const vec3 betaR = vec3(5.5e-6,  13.0e-6, 22.4e-6);
const vec3 betaM = vec3(21e-6);

const float hR = 7994.0;
const float hM = 1200.0;

float rayleight_phase_func(float mu){
    return 3.0 * (1. + mu * mu) / (16. * PI);
}

const float g = 0.76;
float henyey_greenstein_phase_func(float mu){
    return (1.0 - g * g) / ((4.0 * PI) * pow(1.0 + g * g - 2.0 * g * mu, 1.5));
}

float mie_phase_punc(float mu)
{
    return 3.0 * (1.0 - g * g) * (1.0 + mu * mu) / (8.0 * PI) / (2.0 + g * g) / pow(1.0 + g * g - 2.0 * g * mu, 1.5);
}

bool intersects_sphere(vec3 r_org, vec3 r_dir, vec3 s_org, float s_radius, inout float t0, inout float t1){
    vec3 rc = s_org - r_org;
    float radius2 = s_radius * s_radius;
    float tca = dot(rc, r_dir);
    float d2 = dot(rc, rc) - tca * tca;
    if(d2 > radius2)
        return false;
    float thc = sqrt(radius2 - d2);
    t0 = tca - thc;
    t1 = tca + thc;
    return true;
}

const float earth_radius = 6360e3;
const float atmosphere_radius = 6420e3;

const vec3 sun_power = vec3(20.0);
const vec3 moon_power = sun_power * vec3(0.2, 0.2, 0.35);

const int num_samples = 14;
const int num_samples_light = 3;

bool get_sun_light(in vec3 r_org, in vec3 r_dir, inout float opticalDepthR, inout float opticalDepthM){
    float t0, t1;
    intersects_sphere(r_org, r_dir, vec3(0), atmosphere_radius, t0, t1);

    float march_pos = 0;
    float march_length = t1 / float(num_samples_light);

    for(int i=0; i < num_samples_light;i++){
        vec3 s = r_org + r_dir * (march_pos + 0.5 * march_length);
        float height = length(s) - earth_radius;
        opticalDepthR += exp(-height/hR) * march_length;
        opticalDepthM += exp(-height/hM) * march_length;

        march_pos += march_length;
    }

    return true;
}

vec3 get_incident_light(in vec3 r_org, in vec3 r_dir){
    float time = sb.time / 50;
    vec3 sun_dir = sb.dir_light; // normalize(vec3(0, sin(time), cos(time)));
    sun_dir.y *=-1;

    vec3 moon_dir = -sun_dir;

    float t0, t1;
    if(!intersects_sphere(r_org, r_dir, vec3(0), atmosphere_radius, t0, t1))
        return vec3(0.0);

    float march_length = t1 / float(num_samples);

    float mu = dot(sun_dir, r_dir);
    float muMoon = dot(moon_dir, r_dir);

    float phaseR = rayleight_phase_func(mu);
    float phaseM = henyey_greenstein_phase_func(mu);
    float phaseMoonR = rayleight_phase_func(muMoon);
    float phaseMoonM = henyey_greenstein_phase_func(muMoon);

    float opticalDepthR = 0;
    float opticalDepthM = 0;
    float opticalDepthMoonR = 0;
    float opticalDepthMoonM = 0;

    vec3 sumR = vec3(0);
    vec3 sumM = vec3(0);
    vec3 sumMoonR = vec3(0);
    vec3 sumMoonM = vec3(0);

    float march_pos = 0.0;

    for(int i=0; i < num_samples;i++){
        vec3 s = r_org + r_dir * (march_pos + 0.5 * march_length);
        float height = length(s) - earth_radius;

        float hr = exp(-height / hR) * march_length;
        float hm = exp(-height / hM) * march_length;
        opticalDepthR += hr;
        opticalDepthM += hm;

        float opticalDepthLightR = 0;
        float opticalDepthLightM = 0;
        float opticalDepthLightMoonR = 0;
        float opticalDepthLightMoonM = 0;

        bool overground = get_sun_light(s, sun_dir, opticalDepthLightR, opticalDepthLightM);
        bool overgroundMoon = get_sun_light(s, moon_dir, opticalDepthLightMoonR, opticalDepthLightMoonM);

        if(overground || overgroundMoon)
        {
            vec3 tau = betaR * (opticalDepthR + opticalDepthLightR) + betaM * 1.1 * (opticalDepthM + opticalDepthLightM);

            vec3 tau_moon = betaR * (opticalDepthR + opticalDepthLightMoonR) + betaM * 1.1 * (opticalDepthM + opticalDepthLightMoonM);

            vec3 attenuation = exp(-tau);
            vec3 attenuationMoon = exp(-tau_moon);

            sumR += hr * attenuation;
            sumM += hm * attenuation;
            sumMoonR += hr * attenuationMoon;
            sumMoonM += hm * attenuationMoon;
        }

        march_pos += march_length;
    }

    vec3 col = sun_power * (sumR * phaseR * betaR + sumM * phaseM * betaM) + moon_power * (sumMoonR * phaseMoonR * betaR + sumMoonM * phaseMoonM * betaM);

    if(dot(r_dir, sun_dir) > 0.9995) col *= (1.5, 1.5, 1.5);
    if(dot(r_dir, moon_dir) > 0.9995) col *= (1.9, 1.9, 1.9);

    return col;
}

void main() {

    vec4 r_color = vec4(0, 0, 0, 1);

    vec3 cam_pos = vec3(0, earth_radius + 1, 0);
    vec2 uv = fragTexCoord - 0.5;

    mat3 look = look_at3(cam_pos, cam_pos + sb.cam_rot, vec3(0,1,0)) ;
    vec3 dir = normalize(look * uvToCastPlane(uv));
    dir.y *= -1;

    vec3 col = get_incident_light(cam_pos, dir);
    col = pow(col * 5.0 / (col * 5.0 + vec3(1)), vec3(2.2));
    r_color.rgb = col;
    outColor = r_color;

}
