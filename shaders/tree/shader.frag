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

    outColor = color;

}
