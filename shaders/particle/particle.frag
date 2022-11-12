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

void main() {

    vec4 color = texture(texSampler, gl_PointCoord);

    if(color.a < 0.01)
        discard;

    color *= vec4(fragColor, 1.0);

    outColor = color;

}
