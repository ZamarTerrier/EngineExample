#version 450

vec3 LineColor = vec3(1,1,1);

layout(location = 0) out vec4 outColor;

void main(void)
{
    outColor = vec4(LineColor, 0.0);
}
