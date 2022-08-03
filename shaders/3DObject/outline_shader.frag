#version 450

#define DEEP 20

#define pi 3.14159265

layout(binding = 2) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec4 fragVertCoord;
layout(location = 3) in float time;

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

float drawLine (vec2 p1, vec2 p2, vec2 uv, float a)
{
    float r = 0.;

    // get dist between points
    float d = distance(p1, p2);

    // get dist between current pixel and p1
    float duv = distance(p1, uv);

    //if point is on line, according to dist, it should match current uv
    r = 1.-floor(1.-(a* 0.001)+distance (mix(p1, p2, clamp(duv/d, 0., 1.)),  uv));

    return r;
}

float drawCircle(vec2 p, float d, vec2 uv)
{
    return (distance(p, uv) <= d) ? 1. : 0.;
}

vec4 mainImage()
{
    vec2 uv = fragTexCoord.xy;
    float t = time*1.5; //Pro Tip™: multipluy time to go faster!

    //in uv space
    vec2 p1 = vec2(0.5 + sin(t)*.2, 0.25 + cos(t+pi)*.1);
    vec2 p2 = vec2(0.5 + sin(t+pi)*.2, 0.25 + cos(t)*.1);
    vec2 p3 = vec2(0.5 + sin(t+pi/2.)*.2, 0.25 + cos(t-.5*pi)*.1);
    vec2 p4 = vec2(0.5 + sin(t-pi/2.)*.2, 0.25 + cos(t+.5*pi)*.1);
    vec2 p5 = vec2(0.5 + sin(t)*.2, 0.75 + cos(t+pi)*.1);
    vec2 p6 = vec2(0.5 + sin(t+pi)*.2, 0.75 + cos(t)*.1);
    vec2 p7 = vec2(0.5 + sin(t+pi/2.)*.2, 0.75 + cos(t-.5*pi)*.1);
    vec2 p8 = vec2(0.5 + sin(t-pi/2.)*.2, 0.75 + cos(t+.5*pi)*.1);


    float lines = drawLine(p1, p5, uv, 1.)
                          + drawLine(p2, p6, uv, 1.)
                          + drawLine(p1, p3, uv, 1.)
                          + drawLine(p3, p2, uv, 1.)
                          + drawLine(p1, p4, uv, 1.)
                          + drawLine(p4, p2, uv, 1.)
                          + drawLine(p5, p7, uv, 1.)
                          + drawLine(p7, p6, uv, 1.)
                          + drawLine(p6, p8, uv, 1.)
                          + drawLine(p8, p5, uv, 1.)
                          + drawLine(p3, p7, uv, 1.)
                          + drawLine(p4, p8, uv, 1.);

    float d = 0.003;
    float dots = drawCircle(p1, d, uv)
                                 + drawCircle(p2, d, uv)
                         + drawCircle(p3, d, uv)
                                 + drawCircle(p4, d, uv)
                         + drawCircle(p5, d, uv)
                                 + drawCircle(p6, d, uv)
                         + drawCircle(p7, d, uv)
                                 + drawCircle(p8, d, uv);

     return vec4(lines*.6-dots, dots, dots, 1.);
}

void main(void)
{
    vec4 color = texture(texSampler, fragTexCoord);

    vec2 uv = fragTexCoord / color.xy;

    vec3 color2 = fbm_noise(fragTexCoord * 20);

    outColor = mainImage();

    //outColor = vec4(fragColor * color.rgb, 1.0);
}
