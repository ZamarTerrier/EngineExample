#version 450

layout(binding = 1) uniform SDFBuffer{
    mat4 view;
    vec3 obj_pos;
    vec3 obj_rot;
    vec3 cam_pos;
    vec3 cam_rot;
} sdf;

layout(location = 0) in vec2 inUV;
layout(location = 1) in vec4 inWorldCoord;
layout(location = 2) in vec3 inPos;

layout(location = 0) out vec4 outColor;

float sdSphere(vec3 p, float r, vec3 offset)
{
    return length(p - offset) - r;
}

float sdCappedCylinder( vec3 p, float h, float r )
{
    vec2 d = abs(vec2(length(p.xz), p.y)) - vec2(r, h);
    return min(max(d.x, d.y), 0.0) + length(max(d, 0.0));
}

float sdCone( vec3 p, vec2 c, float h)
{
    float q = length(p.xz);
    return max(dot(c.xy, vec2(q, p.y)), -h - p.y);
}

float tree(vec3 pos)
{
    pos.y -=0.5;
    float s = sin(pos.y * 0.03);
    float c = cos(pos.y * 0.03);
    mat2 m = mat2(c, -s, s, c);
    vec3 p = vec3(m * pos.xy, pos.z);

    float width = 0.2 - pos.y * 0.02 - clamp(sin(pos.y * 8.0) * 0.1, 0.05, 0.1);

    return max(length(p.xz) - width, pos.y - 12.5);
}

float merge(float shape1, float shape2){
    return min(shape1, shape2);
}

float intersect(float shape1, float shape2){
    return max(shape1, shape2);
}

float subtract(float base, float subtraction){
    return intersect(base, -subtraction);
}

float scene(vec3 p)
{
    return merge(sdCappedCylinder(p, 0.7, 0.2), sdCone(vec3(p.x, p.y - 1.0, p.z), vec2(0.8, 0.8), 0.7)); //merge(sdSphere(p, 0.7, vec3(0)), tree(p));
}

float rayMarch(vec3 ro, vec3 rd)
{
    float depth = 0.0;
    float d;

    for(int i=0;i < 255; i++)
    {
        vec3 p = ro + depth * rd;
        d = scene(p);
        depth += d;
        if(d < 0.001 || depth > 100.0) break;
    }

    d = depth;

    return d;
}

vec3 calcNormal(in vec3 p){
        vec2 e = vec2(1,-1) * 0.0005;
        return normalize(
          vec3(e.x, e.y, e.y) * scene(p + vec3(e.x, e.y, e.y)) +
          vec3(e.y, e.y, e.x) * scene(p + vec3(e.y, e.y, e.x)) +
          vec3(e.y, e.x, e.y) * scene(p + vec3(e.y, e.x, e.y)) +
          vec3(e.x, e.x, e.x) * scene(p + vec3(e.x, e.x, e.x)));
}

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

mat3 camera(vec3 cameraPos, vec3 lookAtPoint) {
    vec3 cd = normalize(lookAtPoint - cameraPos);
    vec3 cr = normalize(cross(vec3(0, 1, 0), cd));
    vec3 cu = normalize(cross(cd, cr));

    return mat3(-cr, cu, -cd);
}

void main() {

    vec2 uv = inUV - 0.5;
    vec3 col = vec3(0);
    vec3 lp = vec3(0);

    vec3 c_pos = -sdf.cam_pos / 10;

    vec3 rayOrig = camera(sdf.cam_pos, sdf.cam_rot) * sdf.cam_pos;
    vec3 rayDir = normalize(inPos - rayOrig);

    float d = rayMarch(rayOrig, rayDir); // signed distance value to closest object

    vec4 color;

    if(d > 100)
        discard;//color = vec4(vec3(0.2), 1.0);
    else{
        color = vec4(0.4, 0.2, 0.1, 1.0);

        vec3 p = rayOrig + rayDir * d;
        vec3 normal = calcNormal(p);

        vec3 light = vec3(0, 2, 2);
        vec3 lightDir = normalize(light - p) * .65;

        float dif = clamp(dot(normal, lightDir), 0., 1.) * 0.4 + 0.5;

        color.xyz += vec3(dif);
    }

    outColor = color;

}
