#version 450

layout(binding = 0) uniform ModelBuffer3D{
    mat4 model;
    mat4 view;
    mat4 proj;
} mbo;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 color;
layout(location = 3) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec4 fragPos;
layout(location = 3) out vec4 fragNormal;

void main(){

    gl_Position = mbo.proj * mbo.view * mbo.model * vec4(position, 1.0);

    fragColor = color;
    fragTexCoord = inTexCoord;
    fragPos = mbo.model * vec4(position, 1.0);
    fragNormal = mbo.model * vec4(normal, 0.0);
}
