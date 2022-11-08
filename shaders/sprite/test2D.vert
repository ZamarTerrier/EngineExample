#version 450

layout(location = 0) in vec2 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 inTexCoord;

layout(binding = 0) uniform DataBuffer{
    vec2 mouse;
    float time;
    vec3 camRot;
    vec3 camPos;
} data;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out float time;
layout(location = 3) out vec2 mouse;
layout(location = 4) out vec3 rot;
layout(location = 5) out vec3 cPos;

void main(void)
{
    gl_Position = vec4(position, 0.0, 1.0);

    fragColor = color;
    fragTexCoord = inTexCoord;

    time = data.time;
    mouse = data.mouse;

    rot = data.camRot;
    cPos = data.camPos;
}
