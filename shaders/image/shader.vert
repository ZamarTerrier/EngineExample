#version 450

layout(location = 0) in vec2 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec2 fragTexCoord;
layout(location = 1) out float fragDepth;


void main() {

    gl_Position = vec4(position, 0.0, 1.0);

    fragTexCoord = inTexCoord;
    fragDepth = gl_Position.z;
}
