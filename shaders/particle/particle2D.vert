#version 450

layout(binding = 0) uniform TransformBuffer2D{
    vec2 position;
    vec2 rotation;
    vec2 scale;
} tbo;

layout(location = 0) in vec2 position;
layout(location = 1) in float size;
layout(location = 2) in vec3 color;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 pos;

void main(){

    gl_Position = vec4(tbo.position +(position * tbo.scale), 0.0f, 1.0);
    gl_PointSize = 10 * size;

    fragColor = color;
    pos = position;
}
