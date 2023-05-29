#version 450

layout(binding = 0) uniform ModelBuffer3D{
    mat4 model;
    mat4 view;
    mat4 proj;
} mbo;

layout(location = 0) in vec3 position;
layout(location = 1) in float size;
layout(location = 2) in vec3 color;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;

void main(){

    gl_Position = mbo.proj * mbo.view * mbo.model * vec4(position, 1.0);
    gl_PointSize = 200 / gl_Position.z * size;

    fragColor = color;
    pos = position;
}
