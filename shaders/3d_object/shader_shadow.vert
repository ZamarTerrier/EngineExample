#version 450

layout(binding = 0) uniform ModelBuffer3D{
    mat4 model;
    mat4 view;
    mat4 proj;
} mbo;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec2 fragTexCoord;
layout(location = 1) out vec4 fragPos;
layout(location = 2) out vec3 fragNormal;
layout(location = 3) out float fragDepth;
layout(location = 4) out vec4 outModel;
layout(location = 5) out vec3 outWorldPos;
layout(location = 6) out vec4 outViewPos;

void main() {

    gl_Position = mbo.proj * mbo.view * mbo.model * vec4(position, 1.0);
    fragPos = mbo.model * vec4(position, 1.0);

    outModel = mbo.model * vec4(position, 1.0);
    outWorldPos = position;
    outViewPos = mbo.view * outModel;

    fragTexCoord = inTexCoord;
    fragNormal =  transpose(inverse(mat3(mbo.model))) * normal;
    gl_PointSize = 10.0f;
    fragDepth = gl_Position.z;
}
