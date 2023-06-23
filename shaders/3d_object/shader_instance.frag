 
#version 450
layout (binding = 1) uniform sampler2D diffuse;

layout (location = 0) in vec3 inNormal;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inViewVec;
layout (location = 3) in vec3 inLightVec;

layout (location = 0) out vec4 outFragColor;

float fog(float density)
{
        const float LOG2 = -1.442695;
        float dist = gl_FragCoord.z / gl_FragCoord.w * 0.0015;
        float d = density * dist;
        return 1.0 - clamp(exp2(d * d * LOG2), 0.0, 1.0);
}

void main()
{
    vec4 color = texture(diffuse, inUV);

    if(color.a < 0.8)
        discard;

    const vec4 fogColor = vec4(0.47, 0.5, 0.67, 0.0);
    outFragColor = mix(color, fogColor, fog(0.25));
}
