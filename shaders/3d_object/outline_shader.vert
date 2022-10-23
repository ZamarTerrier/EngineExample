#version 450

layout(binding = 0) uniform ModelBuffer3D{
    mat4 model;
    mat4 view;
    mat4 proj;
} mbo;

layout(binding = 1) uniform DataBuffer{
    vec2 mouse;
    float time;
} data;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 color;
layout(location = 3) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec4 fragVertCoord;
layout(location = 3) out float time;

float hash( float n ) {
    return fract(sin(n)*43758.5453);
}

float noise( in vec2 x ) {
    vec2 p = floor(x);
    vec2 f = fract(x);
    f = f*f*(3.0-2.0*f);
    float n = p.x + p.y*57.0;
    float res = mix(mix( hash(n+  0.0), hash(n+  1.0),f.x), mix( hash(n+ 57.0), hash(n+ 58.0),f.x),f.y);
    return res;
}
float fbm( vec2 p ) {
        float f = 0.0;
        f += 0.50000*noise( p ); p = p*2.02;
        f += 0.25000*noise( p ); p = p*2.03;
        f += 0.12500*noise( p ); p = p*2.01;
        f += 0.06250*noise( p ); p = p*2.04;
        f += 0.03125*noise( p );
        return f/0.984375;
}

void main() {


    fragColor = color;
    fragTexCoord = inTexCoord;
    vec4 pos = mbo.model * vec4(position, 1.0);
    //pos.z -= fbm(inTexCoord * data.time * 0.1);
    fragVertCoord = mbo.proj * mbo.view * pos;
    gl_Position = fragVertCoord;
    time = data.time;

}
