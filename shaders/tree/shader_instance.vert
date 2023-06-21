 #version 450

// Vertex attributes
layout (location = 0) in vec3 inPos;
layout (location = 1) in vec3 inNormal;
layout (location = 2) in vec2 inUV;
layout (location = 3) in vec3 inColor;

// Instanced attributes
layout (location = 4) in vec3 instancePos;
layout (location = 5) in vec3 instanceRot;
layout (location = 6) in float instanceScale;

layout(binding = 0) uniform ModelBuffer3D{
    mat4 model;
    mat4 view;
    mat4 proj;
} mbo;

layout(location = 0) out vec2 outUV;
layout(location = 1) out vec3 outColor;

void main()
{
        outUV = inUV;

        mat4 mx, my, mz;

        // rotate around x
        float s = sin(instanceRot.x);
        float c = cos(instanceRot.x);

        mx[0] = vec4(c, s, 0.0, 0.0);
        mx[1] = vec4(-s, c, 0.0, 0.0);
        mx[2] = vec4(0.0, 0.0, 1.0, 0.0);
        mx[3] = vec4(0.0, 0.0, 0.0, 1.0);

        // rotate around y
        s = sin(instanceRot.y);
        c = cos(instanceRot.y);

        my[0] = vec4(c, 0.0, s, 0.0);
        my[1] = vec4(0.0, 1.0, 0.0, 0.0);
        my[2] = vec4(-s, 0.0, c, 0.0);
        my[3] = vec4(0.0, 0.0, 0.0, 1.0);

        // rot around z
        s = sin(instanceRot.z);
        c = cos(instanceRot.z);

        mz[0] = vec4(1.0, 0.0, 0.0, 0.0);
        mz[1] = vec4(0.0, c, s, 0.0);
        mz[2] = vec4(0.0, -s, c, 0.0);
        mz[3] = vec4(0.0, 0.0, 0.0, 1.0);

        mat4 rotMat = mz * my * mx;

        vec4 pos = vec4(inPos.xyz * instanceScale, 1.0) * rotMat;
        pos.xyz += instancePos;

        gl_Position = mbo.proj * mbo.view * mbo.model * pos;

        outUV = inUV;
        outColor = inColor;
}
