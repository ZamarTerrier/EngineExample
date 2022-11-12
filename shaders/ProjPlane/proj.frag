#version 450

float PI = 3.14159265359;

layout(binding = 0) uniform ProjDataBuffer{
    float time;
    vec3 camRot;
    vec3 camPos;
} data;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

#define CAMERA_FOV 1.1

mat3 look_at3(vec3 from, vec3 to, vec3 up){

    vec3 z = normalize(to - from);
    vec3 x = normalize(cross(up, z));
    vec3 y = normalize(cross(z, x));

    mat3 result = mat3(
        vec3(x.x, x.y, x.z),
        vec3(y.x, y.y, y.z),
        vec3(z.x, z.y, z.z));

    return result;
}


vec3 uvToCastPlane(vec2 uv)
{
        return vec3(uv.x - 0.5, uv.y - 0.5, CAMERA_FOV);
}


float sphereSDF(vec3 coord, vec3 sphereCenter, float radius)
{
        return distance(coord, sphereCenter) - radius;
}

float objectSDF(vec3 coord)
{
        float obj = sphereSDF(coord, vec3(0.0, 5.0, 10.0), 1.0);
    obj = min(obj, sphereSDF(coord, vec3(-1.0, 10.0, 14.4), 0.5));
    return obj;
}

vec3 rayMarch(vec3 rayOrigin, vec3 rayDirection)
{
        for (int i = 0; i < 40; i++)
    {
        float sdf = objectSDF(rayOrigin);
        rayOrigin += sdf * rayDirection * 0.8;
    }

    return rayOrigin;
}

vec3 objectNormal(vec3 coord)
{
    float eps = 0.001;
        float v000 = objectSDF(coord);
    float v100 = objectSDF(coord + vec3(eps, 0.0, 0.0));
    float v010 = objectSDF(coord + vec3(0.0, eps, 0.0));
    float v001 = objectSDF(coord + vec3(0.0, 0.0, eps));

    return normalize(vec3(v100 - v000, v010 - v000, v001 - v000));
}

void main(void)
{


    //Getting camera properties from buffer A
    vec3 cameraPos = vec3(0);
    vec3 cameraRotation = data.camRot;

    vec4 color = vec4(0.0);

        //Creating ray
    vec3 rayOrigin = -cameraPos;
    mat3 look = look_at3(cameraPos, cameraPos + cameraRotation, vec3(0,1,0)) ;
    vec3 rayDirection = normalize(look * uvToCastPlane(fragTexCoord));

    vec3 hitPoint = rayMarch(rayOrigin, rayDirection);
    float sdf = objectSDF(hitPoint);

    bool isObject = sdf < 0.01;
    if (isObject)
    {
        vec3 normal = objectNormal(hitPoint);
        color.rgb += normal;
    }
    else
    {

    }

    //Getting camera properties from buffer A

    outColor = vec4(color.rgb, 1.0);
}
