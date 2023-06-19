 
#version 450
layout (binding = 1) uniform sampler2D diffuse;

layout (location = 0) in vec3 inNormal;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inViewVec;
layout (location = 3) in vec3 inLightVec;

layout (location = 0) out vec4 outFragColor;

void main()
{
    vec4 color = texture(diffuse, inUV);

    outFragColor = vec4(color.rgb, 1.0);
}
