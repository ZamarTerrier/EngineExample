#version 450

layout(binding = 0) uniform ModelBuffer3D{
    mat4 model;
    mat4 view;
    mat4 proj;
} mbo;

layout(binding = 1) uniform Matrixes{
    mat4 mats[64];
    int size;
} mato;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 color;
layout(location = 3) in vec2 inTexCoord;
layout(location = 4) in vec4 joints;
layout(location = 5) in vec4 weight;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec4 fragPos;
layout(location = 3) out vec4 fragNormal;

void main() {

    mat4 model = mbo.model;

    mat4 skinMat = weight.x * mato.mats[int(joints.x)] +  weight.y * mato.mats[int(joints.y)] + weight.z * mato.mats[int(joints.z)] + weight.w * mato.mats[int(joints.w)];

    if(mato.size > 0)
        model = skinMat * mbo.model;

    gl_Position = mbo.proj * mbo.view * model * vec4(position, 1.0);

    fragColor = color;
    fragTexCoord = inTexCoord;
    fragPos =mbo.model * vec4(position, 1.0);
    fragNormal = mbo.model * vec4(normal, 0.0);
    gl_PointSize = 10.0f;
}
