#version 450

layout(binding = 1) uniform ImageBufferObjects {
    vec2 origin;
    vec2 offset;
    vec2 rotation;
    vec2 scale;
} tbo;

layout(binding = 2) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main() {

    vec2 origin = fragTexCoord;

    if(origin.x < tbo.origin.x || origin.y < tbo.origin.y)
        discard;
    else if(origin.x > tbo.scale.x || origin.y > tbo.scale.y)
        discard;

    vec4 color = texture(texSampler, fragTexCoord + tbo.offset);

    outColor = vec4(fragColor * color.rgb, color.a);
}
