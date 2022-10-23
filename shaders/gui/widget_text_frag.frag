#version 450

float PI = 3.14159265359;

#define MAX_MASKS_OBJECTS 256

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main(void)
{

    //Getting camera properties from buffer A

   vec4 color = texture(texSampler, fragTexCoord);

   outColor = vec4(fragColor, color.r);
}
