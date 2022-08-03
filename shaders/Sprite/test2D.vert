#version 450

layout(location = 0) in vec2 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 inTexCoord;

layout(binding = 0) uniform DataBuffer{
    float time;
} data;


layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out float time;

void main(void)
{
    gl_Position = vec4(position, 0.0, 1.0);

    fragColor = color;
    fragTexCoord = inTexCoord;

    time = data.time;
}
