#version 450


layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main() {
    float depthValue = texture(texSampler, fragTexCoord).r;
    outColor = vec4(vec3(depthValue), 1.0);
}
