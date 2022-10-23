#version 450

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 color;
layout(location = 3) in vec2 inTexCoord;

layout(binding = 0) uniform ModelBuffer3D{
    mat4 model;
    mat4 view;
    mat4 proj;
} mbo;

layout(location = 0) out vec4 worldCoord;
layout(location = 1) out vec4 worldNormal;
layout(location = 2) out vec4 posCoord;
layout(location = 3) out vec3 fragColor;
layout(location = 4) out vec2 fragTexCoord;

void main(void)
{
    // Position
    worldCoord = mbo.model * vec4(position, 1.0);

    posCoord = vec4(position, 1.0);

    // Normal
    worldNormal = normalize(vec4(normal, 0.0));

    gl_Position = mbo.proj * mbo.view * mbo.model * vec4(position, 1.0);
}
