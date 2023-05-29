#version 450


layout(location = 0) in vec2 inUV;

layout(location = 0) out vec4 outColor;

void main() {

    vec4 color = vec4(vec3(0.7), 1.0);

    outColor = color;

}
