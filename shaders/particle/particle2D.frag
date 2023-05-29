#version 450

layout(binding = 1) uniform sampler2D diffuse;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 pos;

layout(location = 0) out vec4 outColor;

void main() {

    vec4 color = texture(diffuse, gl_PointCoord);

    if(color.a < 0.01)
        discard;

    color *= vec4(fragColor, 1.0);

    outColor = color;

}
