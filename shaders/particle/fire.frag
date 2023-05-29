#version 450

layout(binding = 1) uniform DataBuffer{
    vec2 mouse;
    float time;
    vec3 camRot;
    vec3 camPos;
} data;

layout(binding = 2) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec3 pos;

layout(location = 0) out vec4 outColor;

vec2 hash( vec2 p ) {
        p = vec2(dot(p,vec2(127.1,311.7)), dot(p,vec2(269.5,183.3)));
        return -1.0 + 2.0*fract(sin(p)*43758.5453123);
}

float noise( in vec2 p ) {
    const float K1 = 0.366025404; // (sqrt(3)-1)/2;
    const float K2 = 0.211324865; // (3-sqrt(3))/6;
        vec2 i = floor(p + (p.x+p.y)*K1);
    vec2 a = p - i + (i.x+i.y)*K2;
    vec2 o = (a.x>a.y) ? vec2(1.0,0.0) : vec2(0.0,1.0); //vec2 of = 0.5 + 0.5*vec2(sign(a.x-a.y), sign(a.y-a.x));
    vec2 b = a - o + K2;
        vec2 c = a - 1.0 + 2.0*K2;
    vec3 h = max(0.5-vec3(dot(a,a), dot(b,b), dot(c,c) ), 0.0 );
        vec3 n = h*h*h*h*vec3( dot(a,hash(i+0.0)), dot(b,hash(i+o)), dot(c,hash(i+1.0)));
    return dot(n, vec3(70.0));
}

float fbm(vec2 n) {
    float total = 0.0, amplitude = 1.0;
    for (int i = 0; i <7; i++) {
        total += noise(n) * amplitude;
        n += n*1.7;
        amplitude *= 0.47;
    }
    return total;
}



void main() {

    vec4 tColor = texture(texSampler, gl_PointCoord);

    vec3 color = vec3(0);

    vec2 uv = gl_PointCoord;

    float dist = distance(uv, vec2(0.5, 0.5));

    float t = abs(fbm(vec2(uv.x + pos.x, (uv.y + pos.y) + (data.time * 2))) - (dist * 2) - (pos.y));

    color = vec3(1.0, 0.2, 0.05) / (t * 2) * tColor.rgb;

    if(length(color) < 1.5)
        discard;

    outColor = vec4(color.rgb, 1.0);

}
