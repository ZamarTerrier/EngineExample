#version 450

layout(binding = 1) uniform SomeBufferObject{
    float time;
} sbo;

layout(binding = 2) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec4 fragPos;
layout(location = 3) in vec4 fragNormal;

layout(location = 0) out vec4 outColor;

void main()
{
    vec2 uv = fragTexCoord;

    uv.x += sbo.time;
    vec4 color = texture(texSampler, uv);


    outColor = vec4(color.rgb + fragColor, 1.0);
}
