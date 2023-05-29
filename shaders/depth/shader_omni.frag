#version 450

layout (location = 0) in vec4 fragPos;
layout (location = 1) in vec3 fragLightPos;

void main() {

    // Store distance to light as 32 bit float value
    vec3 orig = (fragPos / fragPos.w).xyz;
    vec3 someDirLight = orig - fragLightPos;
    float lightDistance = length(someDirLight);

    lightDistance /= 100;

    gl_FragDepth = lightDistance;

}
