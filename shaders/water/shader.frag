#version 450

layout(binding = 1) uniform WaterBuffer {
    float time;
} wb;

layout(binding = 2) uniform sampler2D diffuse;

layout(location = 0) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main() {

    float s_val = (sin(wb.time) + 1) / 100;
    float c_val = (cos(wb.time) + 1) / 100;

    vec2 offset = vec2(s_val, c_val);

    float time = wb.time / 200;

    vec4 color = texture(diffuse, fragTexCoord + offset + time) / 40;

    color.a = 0.95;

    outColor = color;

}
