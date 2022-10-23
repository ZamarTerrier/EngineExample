#version 450

layout(binding = 0) uniform TransformBufferObjects {
    vec2 position;
    vec2 rotation;
    vec2 scale;
} tbo;

layout(location = 0) in vec2 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

void main(void)
{
    gl_Position = vec4(tbo.position + (position * tbo.scale),  0.0, 1.0);
    fragColor = color;
    fragTexCoord = inTexCoord;

}
