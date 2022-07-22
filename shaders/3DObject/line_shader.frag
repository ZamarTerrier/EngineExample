#version 450

layout(location = 0) out vec4 outColor;

vec3 LineColor = vec3(1,1,1);

void main(void)
{
    outColor = vec4(LineColor, 0.0);
}
