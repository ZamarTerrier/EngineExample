#version 450

const float PI = 3.14159265359;

layout(binding = 1) uniform SomeData{
    mat4 view;
    vec3 objPos;
    vec3 objRot;
    vec3 camPos;
    vec3 camRot;
} c;

layout(binding = 2) uniform DataBuffer{
    vec2 mouse;
    float time;
    vec3 camRot;
    vec3 camPos;
} data;

//layout(binding = 3) uniform sampler2D texSampler;

layout(location = 0) in vec4 worldCoord;
layout(location = 1) in vec4 worldNormal;
layout(location = 2) in vec4 posCoord;
layout(location = 3) in vec3 fragColor;
layout(location = 4) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

mat3 rotateX(float theta) {
    theta *= PI / 180;  // convert to radians

    float c = cos(theta);
    float s = sin(theta);
    return mat3(
        vec3(1, 0, 0),
        vec3(0, c, -s),
        vec3(0, s, c)
    );
}

// Rotation matrix around the Y axis.
mat3 rotateY(float theta) {
    theta *= PI / 180;  // convert to radians

    float c = cos(theta);
    float s = sin(theta);
    return mat3(
        vec3(c, 0, s),
        vec3(0, 1, 0),
        vec3(-s, 0, c)
    );
}

// Rotation matrix around the Z axis.
mat3 rotateZ(float theta) {
    theta *= PI / 180;  // convert to radians

    float c = cos(theta);
    float s = sin(theta);
    return mat3(
        vec3(c, -s, 0),
        vec3(s, c, 0),
        vec3(0, 0, 1)
    );
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

float interpolate(float shape1, float shape2, float amount){
    return mix(shape1, shape2, amount);
}

float round_merge(float shape1, float shape2, float radius){
    vec2 intersectionSpace = vec2(shape1 - radius, shape2 - radius);
    intersectionSpace = min(intersectionSpace, 0);
    float insideDistance = -length(intersectionSpace);
    float simpleUnion = merge(shape1, shape2);
    float outsideDistance = max(simpleUnion, radius);
    return  insideDistance + outsideDistance;
}

float round_intersect(float shape1, float shape2, float radius){
    vec2 intersectionSpace = vec2(shape1 + radius, shape2 + radius);
    intersectionSpace = max(intersectionSpace, 0);
    float outsideDistance = length(intersectionSpace);
    float simpleIntersection = intersect(shape1, shape2);
    float insideDistance = min(simpleIntersection, -radius);
    return outsideDistance + insideDistance;
}

float round_subtract(float base, float subtraction, float radius){
    return round_intersect(base, -subtraction, radius);
}

float sdSphere(vec3 orig, vec3 off, float radius){
    vec3 com = orig - off;
    //get distance from center and grow it according to radius
    return length(com) - radius;
}

float circle(vec2 orig, vec2 off, float radius){
    vec2 com = orig - off;
    //get distance from center and grow it according to radius
    return length(com) - radius;
}

float sphIntersect( vec3 ro, vec3 rd, vec4 sph )
{
    vec3 oc = ro - sph.xyz;
    float b = dot( oc, rd );
    float c = dot( oc, oc ) - sph.w*sph.w;
    float h = b*b - c;
    if( h<0.0 ) return -1.0;
    h = sqrt( h );
    return -b - h;
}

mat4 look_at(vec3 from, vec3 to, vec3 up){

    vec3 z = normalize(to - from) * -1;
    vec3 x = normalize(cross(up, z));
    vec3 y = cross(z, x);

    mat4 result = mat4(
        vec4(x.x, x.y, x.z, -dot(from, x)),
        vec4(y.x, y.y, y.z, -dot(from, y)),
        vec4(z.x, z.y, z.z, -dot(from, z)),
        vec4(0,   0,   0,    1));

    return result;
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

const int MAX_MARCHING_STEPS = 255;
const float MIN_DIST = 0.0;
const float MAX_DIST = 100.0;
const float PRECISION = 0.001;
const float EPSILON = 0.0005;
const vec3 COLOR_AMBIENT = vec3(0.42, 0.20, 0.1);
const vec3 COLOR_BACKGROUND = vec3(.741, .675, .82);

float sdSphere(vec3 p, float r, vec3 offset)
{
  return length(p - offset) - r;
}

float udBox( vec3 p, vec3 b )
{

    p = p * rotateX(-c.objRot.x) * rotateY(-c.objRot.y) * rotateZ(-c.objRot.z);


    return length(max(abs(p)-b,0.0));
}

float scene(vec3 p) {
    float b = udBox(p, vec3(0.4, 0.4, 0.4)) ;
    const float del = 2.6;
    float s = sdSphere(p + vec3(sin(data.time) / del, cos(data.time) / del, cos(data.time) / del), .3, vec3(0, 0, 0));

    float m = round_merge(b, s, 0.1);

    return m;
}

vec3 calcNormal(in vec3 p) {
    vec2 e = vec2(1, -1) * EPSILON;
    return normalize(
      vec3(e.x, e.y, e.y) * scene(p + vec3(e.x, e.y, e.y)) +
      vec3(e.y, e.y, e.x) * scene(p + vec3(e.y, e.y, e.x)) +
      vec3(e.y, e.x, e.y) * scene(p + vec3(e.y, e.x, e.y)) +
      vec3(e.x, e.x, e.x) * scene(p + vec3(e.x, e.x, e.x)));
}

mat3 camera(vec3 cameraPos, vec3 lookAtPoint) {
        vec3 cd = normalize(lookAtPoint - cameraPos);
        vec3 cr = normalize(cross(vec3(0, 1, 0), cd));
        vec3 cu = normalize(cross(cd, cr));

        return mat3(-cr, cu, -cd);
}

float rayMarch(vec3 ro, vec3 rd) {
  float depth = MIN_DIST;
  float d; // distance ray has travelled

  for (int i = 0; i < MAX_MARCHING_STEPS; i++) {
    vec3 p = ro + depth * rd;
    d = scene(p);
    depth += d;
    if (d < PRECISION || depth > MAX_DIST) break;
  }

  d = depth;

  return d;
}

void main(void)
{

    vec3 col = vec3(0);
      vec3 lp = vec3(0);
      vec4 rayOrig = (inverse(c.view) * vec4(-c.camPos / 10, 1.0));
      vec3 ro = rayOrig.xyz - c.objPos;

      vec4 coord = worldCoord;
      vec4 rayDir = normalize(coord - rayOrig);
      vec3 rd = rayDir.xyz; //rayDir.xyz; // ray direction

      float d = rayMarch(ro, rd); // signed distance value to closest object

      if (d > MAX_DIST) {
        col = vec3(0); // ray didn't hit anything
      } else {
        vec3 p = ro + rd * d; // point discovered from ray marching
        vec3 normal = calcNormal(p); // surface normal

        vec3 lightPosition = vec3(0, 2, 2);
        vec3 lightDirection = normalize(lightPosition - p) * .65; // The 0.65 is used to decrease the light intensity a bit

        float dif = clamp(dot(normal, lightDirection), 0., 1.) * 0.5 + 0.5; // diffuse reflection mapped to values between 0.5 and 1.0

        col = vec3(dif) + COLOR_AMBIENT;
      }


    outColor = vec4(col, 1.0);
}
