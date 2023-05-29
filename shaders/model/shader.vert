#version 450

layout(binding = 0) uniform ModelBuffer3D{
    mat4 model;
    mat4 view;
    mat4 proj;
} mbo;

layout(binding = 1) uniform Matrixes{
    mat4 mats[128];
    int size;
} mato;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec4 joints;
layout(location = 4) in vec4 weight;

layout(location = 0) out vec2 fragTexCoord;
layout(location = 1) out vec3 fragNormal;
layout(location = 2) out vec4 fragPos;
layout(location = 3) out vec4 outModel;
layout(location = 4) out float fragDepth;
layout(location = 5) out vec3 outWorldPos;
layout(location = 6) out vec4 outViewPos;

void main() {

    vec4 model = mbo.model * vec4(position, 1.0);

    mat4 skinMat = weight.x * mato.mats[int(joints.x)] +  weight.y * mato.mats[int(joints.y)] + weight.z * mato.mats[int(joints.z)] + weight.w * mato.mats[int(joints.w)];

    if(mato.size > 0)
        model = mbo.model * (skinMat * vec4(position, 1.0));

    gl_Position = mbo.proj * mbo.view * model;

    outModel = model;
    outWorldPos = position;
    outViewPos = mbo.view * model; //lp.view_pos;

    fragPos = model;

    fragTexCoord = inTexCoord;
    fragNormal =  transpose(inverse(mat3(mbo.model))) * normal;
    gl_PointSize = 10.0f;
    fragDepth = gl_Position.z;

}
