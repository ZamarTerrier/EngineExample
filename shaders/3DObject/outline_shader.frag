#version 450

#define DEEP 20

layout(binding = 2) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec4 fragVertCoord;

vec2 pointPos = vec2(0.6, 0.6);

layout(location = 0) out vec4 outColor;

float random(vec2 uv) {
        return fract(sin(dot(uv.xy,vec2(13.0909,783.342))) * 423234.323);
}
vec2 random2(vec2 uv){
    float res = fract(sin(dot(uv.xy,vec2(132.090,989.232))) * 232324.123);
        return vec2(res);
}
vec3 cellNoise(vec2 uv) {
        uv *= 3.0;
    vec2 i_st = floor(uv);
    vec2 f_st = fract(uv);

    /*vec2 point = random2(i);
    vec2 dist = point - f;
    float di = length(dist);
    */
    vec3 color = vec3(0.);
    float m_dist = 1.;
    for(int i = -1; i <=1; i++) {
        for(int j = -1; j <= 1; j++) {
            vec2 neighbor = vec2(float(j),float(i));
                vec2 point = random2(i_st + neighbor);
            point = 0.5 + 0.5 * sin(1000 + 6.283 * point);
            //  The distance from a pixel to a random point
            vec2 diff = neighbor + point - f_st;
            float dist = length(diff);
            m_dist = min(m_dist,dist);
        }
    }
    color += m_dist;
    return color;
}
vec3 fbm_noise(vec2 uv) {
    vec3 res = vec3(0.);
    float a = 0.0;
    float b = 0.0;
    for(int i = 0; i < DEEP; i++) {
        a += (float(i) + 1.);
        b = 1. / a;
        res += b * cellNoise(a * uv);
    }
        return res;
}

vec4 toBezier(float delta, int i, vec4 P0, vec4 P1, vec4 P2, vec4 P3)
{
    float t = delta * float(i);
    float t2 = t * t;
    float one_minus_t = 1.0 - t;
    float one_minus_t2 = one_minus_t * one_minus_t;
    return (P0 * one_minus_t2 * one_minus_t + P1 * 3.0 * t * one_minus_t2 + P2 * 3.0 * t2 * one_minus_t + P3 * t2 * t);
}

void main(void)
{
    vec4 color = texture(texSampler, fragTexCoord);

    vec2 uv = fragTexCoord / color.xy;

    vec3 color2 = fbm_noise(fragTexCoord * 20);

    float dist = distance(fragTexCoord, pointPos);

    if(dist > 0.3)
        discard;

    /*if(color.a < 0.1)
        discard;*/

    outColor = vec4(fragColor * color.rgb, 1.0);
}
