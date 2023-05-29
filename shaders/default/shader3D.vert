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

void main() {

    gl_Position = mbo.proj * mbo.view * mbo.model * vec4(position, 1.0);

    gl_PointSize = 10.0f;

    outUV = inTexCoord;
}
