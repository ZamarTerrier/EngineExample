#version 450

layout(binding = 3) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec2 inOffset;
layout(location = 3) in vec2 inScale;

layout(location = 0) out vec4 outColor;
float _Rotation = 0.0f;

void main() {

    vec4 color = texture(texSampler, fragTexCoord);

    float sinX = sin ( _Rotation );
    float cosX = cos ( _Rotation );
    float sinY = sin ( _Rotation );
    mat2 rotationMatrix = mat2( cosX, -sinX, sinY, cosX);

    vec4 color2 = texture(texSampler, ((fragTexCoord + inOffset) * inScale) * rotationMatrix);

    if(color2.a < 0.1)
        discard;

    outColor = vec4(fragColor * color2.rgb, 1.0);
}
