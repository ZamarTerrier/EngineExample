#version 450

layout(binding = 0) uniform ModelBuffer3D{
    mat4 model;
    mat4 view;
    mat4 proj;
} mbo;

layout(binding = 1) uniform Matrixes{
    mat4 mats[64];
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

mat4 m4_mult_transform(mat4 m1, mat4 m2)
{
    for (int i = 0; i < 4; ++i)
    {
        float l0 = m1[i][0];
        float l1 = m1[i][1];
        float l2 = m1[i][2];

        float r0 = l0 * m2[0][0] + l1 * m2[1][0] + l2 * m2[2][0];
        float r1 = l0 * m2[0][1] + l1 * m2[1][1] + l2 * m2[2][1];
        float r2 = l0 * m2[0][2] + l1 * m2[1][2] + l2 * m2[2][2];

        m1[i][0] = r0;
        m1[i][1] = r1;
        m1[i][2] = r2;
    }

    m1[3][0] += m2[3][0];
    m1[3][1] += m2[3][1];
    m1[3][2] += m2[3][2];

    return m1;
}

void main() {

    mat4 skinMat = weight.x * mato.mats[int(joints.x)] +  weight.y * mato.mats[int(joints.y)] + weight.z * mato.mats[int(joints.z)] + weight.w * mato.mats[int(joints.w)];

    mat4 model = m4_mult_transform(skinMat, mbo.model);

    gl_Position = mbo.proj * mbo.view * model * vec4(position, 1.0);

    fragColor = color;
    fragTexCoord = inTexCoord;
    fragPos =mbo.model * vec4(position, 1.0);
    fragNormal = mbo.model * vec4(normal, 0.0);
    gl_PointSize = 10.0f;
}
