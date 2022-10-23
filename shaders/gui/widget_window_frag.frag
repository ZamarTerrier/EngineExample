#version 450

float PI = 3.14159265359;

#define MAX_MASKS_OBJECTS 256

layout(binding = 0) uniform GUIBuffer{
    vec2 offset;
    vec2 position;
    vec2 size;
    vec3 color;
} guib;


layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

float sdBox( vec2 p, vec2 off,  vec2 b )
{
    vec2 org = p - (off + b);
    vec2 d = abs(org)-b;
    float outsideDistance = length(max(d, 0));
    float insideDistance = min(max(d.x, d.y), 0);
    return outsideDistance + insideDistance;
}

void main(void)
{

    //Getting camera properties from buffer A

    vec2 size = guib.size;
    vec2 position = guib.position;

    float d = sdBox(fragTexCoord, position, size);

    vec3 color = vec3(0);

    if(d > 0)
    {
        discard;
    }

    color = guib.color;

    outColor = vec4(color, 1.0);
}
