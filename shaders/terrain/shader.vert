#version 450


layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 inTexCoord;

layout (location = 0) out vec3 outNormal;
layout (location = 1) out vec2 outUV;

void main(void)
{
        gl_Position = vec4(position.xyz, 1.0);
        outUV = inTexCoord;
        outNormal = normal;
}
