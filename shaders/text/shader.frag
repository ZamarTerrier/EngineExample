#version 450

layout(binding = 3) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec2 inOffset;
layout(location = 3) in vec2 inScale;


layout(location = 0) out vec4 outColor;

float outline_thickness = .2;
vec3 outline_colour = vec3(1, 0, 1);
float outline_threshold = .5;

void main() {

    vec2 mutFragTexCoord = (fragTexCoord + inOffset) * inScale;

    float color = texture(texSampler, mutFragTexCoord).r;

    outColor = vec4(fragColor * color, color);
}
