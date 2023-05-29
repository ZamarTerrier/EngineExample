#version 450

layout(binding = 0) uniform ModelBuffer3D{
    mat4 model;
    mat4 view;
    mat4 proj;
} mbo;

layout(binding = 1) uniform LightPos{
    vec3 light_pos;
    vec3 view_pos;
} lp;

layout(push_constant) uniform PushConsts
{
    mat4 view;
} pushConsts;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec4 fragPos;
layout(location = 1) out vec3 fragLightPos;

void main() {

    gl_Position = mbo.proj * pushConsts.view * mbo.model * vec4(position, 1.0);

    fragPos = mbo.model * vec4(position, 1.0);
    fragLightPos = vec4(lp.light_pos, 1.0).xyz;
}
