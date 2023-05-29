#version 450

layout(binding = 0) uniform ModelBuffer3D{
    mat4 model;
    mat4 view;
    mat4 proj;
} mbo;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec2 outUV;
layout(location = 1) out vec4 outWorldCoord;
layout(location = 2) out vec3 outPos;

void main() {

    outPos = position;

    gl_Position = mbo.proj * mbo.view * mbo.model * vec4(position, 1.0);

    outWorldCoord = mbo.model * vec4(position, 1.0);

    gl_PointSize = 10.0f;

    outUV = inTexCoord;
}
