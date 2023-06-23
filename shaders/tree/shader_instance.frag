 #version 450

layout(location = 0) in vec2 inUV;
layout(location = 1) in vec3 inColor;

layout(location = 0) out vec4 outColor;

float sdEgg(vec2 p, float ra, float rb)
{
    const float k=sqrt(3.0);
    p.x = abs(p.x);
    float r = ra - rb;
    return ((p.y < 0.0) ? length(vec2(p.x, p.y)) - r : (k * (p.x + r) < p.y) ? length(vec2(p.x, p.y - k * r)) : length(vec2(p.x + r, p.y)) - 2.0 * r) - rb;
}

float fog(float density)
{
        const float LOG2 = -1.442695;
        float dist = gl_FragCoord.z / gl_FragCoord.w * 0.0015;
        float d = density * dist;
        return 1.0 - clamp(exp2(d * d * LOG2), 0.0, 1.0);
}

void main() {

    vec4 color = vec4(inColor, 1.0);

    float d = sdEgg(inUV - 0.5, 0.3, 0.1);

    if(d > 0 && inColor.x < 0.5)
    {
        discard;
    }

    if(inColor.x < 0.5)
        color.rgb *= abs(d);
    else
        color.rgb *= abs(d) / 10;

    const vec4 fogColor = vec4(0.47, 0.5, 0.67, 0.0);
    outColor = mix(color, fogColor, fog(0.25));

}

