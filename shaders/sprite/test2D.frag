#version 450

float PI = 3.14159265359;

#define WIDTH 1024
#define HEIGHT 800

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in float time;
layout(location = 3) in vec2 mouse;
layout(location = 4) in vec3 rot;
layout(location = 5) in vec3 cPos;

vec2 tempMouse;

vec2 dotPos = vec2(0.5, 0.5);
vec2 viewOffset = vec2(0,0);
vec2 viewSize = vec2(WIDTH, HEIGHT);


vec3 spherePos = vec3(0.3, 0.3, 0.3);

mat4 edenMat = mat4(
    vec4(1,0,0,0),
    vec4(0,1,0,0),
    vec4(0,0,1,0),
    vec4(0,0,0,1)
);

mat3 edenMat2d = mat3(
    vec3(1,0,0),
    vec3(0,1,0),
    vec3(0,0,1)
);

layout(location = 0) out vec4 outColor;

float drawLine (vec2 p1, vec2 p2, vec2 uv, float a)
{
    float r = 0.;

    // get dist between points
    float d = distance(p1, p2);

    // get dist between current pixel and p1
    float duv = distance(p1, uv);

    //if point is on line, according to dist, it should match current uv
    r = 1.-floor(1.-(a * 0.001)+distance (mix(p1, p2, clamp(duv/d, 0., 1.)),  uv));

    return r;
}

float drawDot(vec2 p, float d, vec2 uv)
{
    return (distance(p, uv) <= d) ? 1.0 : 0.0;
}

vec3 toBezier(float delta, int i, vec3 P0, vec3 P1, vec3 P2)
{
    float t = delta * float(i);
    float t2 = t * t;
    float one_minus_t = 1.0 - t;
    float one_minus_t2 = one_minus_t * one_minus_t;
    return one_minus_t * (one_minus_t * P0 + t * P1) + t * (one_minus_t * P1 + t * P2);
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


mat4 m4_rotation_matrix(mat4 matrix, vec3 degrees)
{

  degrees.x *= PI / 180;  // convert to radians
  degrees.y *= PI / 180;
  degrees.z *= PI / 180;

  float sin_x = sin(degrees.x);
  float cos_x = cos(degrees.x);
  float sin_y = sin(degrees.y);
  float cos_y = cos(degrees.y);
  float sin_z = sin(degrees.z);
  float cos_z = cos(degrees.z);

  mat4 result;

  result[0][0] = cos_z * cos_y;
  result[1][0] = cos_z * sin_y * sin_x + sin_z * cos_x;
  result[2][0] = -cos_z * sin_y * cos_x + sin_z * sin_x;
  result[3][0] = 0;

  result[0][1] = -sin_z * cos_y;
  result[1][1] = -sin_z * sin_y * sin_x + cos_z * cos_x;
  result[2][1] = sin_z * sin_y * cos_x + cos_z * sin_x;
  result[3][1] = 0;

  result[0][2] = sin_y;
  result[1][2] = -cos_y * sin_x;
  result[2][2] = cos_y * cos_x;
  result[3][2] = 0;

  result[0][3] = 0;
  result[1][3] = 0;
  result[2][3] = 0;
  result[3][3] = 1;

  return result;
}

mat4 m4_perspective(float fov_degrees, float near_plane, float far_plane){

  float aspect_ratio = WIDTH / HEIGHT;

  float range = far_plane  - near_plane;
  float tan_half_fov = tan((fov_degrees * (PI / 180)) / 2.0);

  mat4 matrix;

  matrix[0][0] = 1.0f / (tan_half_fov * aspect_ratio);
  matrix[0][1] = 0.0f;
  matrix[0][2] = 0.0f;
  matrix[0][3] = 0.0f;

  matrix[1][0] = 0.0f;
  matrix[1][1] = 1.0f / tan_half_fov;
  matrix[1][2] = 0.0f;
  matrix[1][3] = 0.0f;

  matrix[2][0] = 0.0f;
  matrix[2][1] = 0.0f;
  matrix[2][2] = far_plane / range;
  matrix[2][3] = 1.0f;

  matrix[3][0] = 0.0f;
  matrix[3][1] = 0.0f;
  matrix[3][2] = -(far_plane * near_plane) / range;
  matrix[3][3] = 0.0f;

  return matrix;
}

mat4 m4_scale(vec3 scale){
    //scale
    mat4 scale_mat = edenMat;

    scale_mat[0][0] = scale.x;
    scale_mat[1][1] = scale.y;
    scale_mat[2][2] = scale.z;

    return scale_mat;
}

mat4 m4_translate(vec3 pos){
    mat4 trans_mat = edenMat;
    //translate
    trans_mat[3][0] += pos.x;
    trans_mat[3][1] += pos.y;
    trans_mat[3][2] += pos.z;

    return trans_mat;
}

vec2 calcLine(){

    vec2 linePos;

    float amplitude = 1.;
    float frequency = 1.;
    linePos.y = sin(linePos.x * frequency);
    float t = 0.01*(-time*130.0);
    linePos.y += sin(linePos.x*frequency*2.1 + t)*4.5;
    linePos.y += sin(linePos.x*frequency*1.72 + t*1.121)*4.0;
    linePos.y += sin(linePos.x*frequency*2.221 + t*0.437)*5.0;
    linePos.y += sin(linePos.x*frequency*3.1122+ t*4.269)*2.5;
    linePos.y *= amplitude*0.06;

    return linePos;
}

int sphereVisibility( in vec3 ca, in float ra, in vec3 cb, float rb, in vec3 c )
{
    float aa = dot(ca-c,ca-c);
    float bb = dot(cb-c,cb-c);
    float ab = dot(ca-c,cb-c);

    float s = ab*ab + ra*ra*bb + rb*rb*aa - aa*bb;
    float t = 2.0*ab*ra*rb;

             if( s + t < 0.0 ) return 1;
        else if( s - t < 0.0 ) return 2;
                               return 3;
}

float modelSphere(vec3 pos, vec3 sph )
{
    vec3 temp = vec3(sph.xy, 0);
    vec3 di = pos - sph;
    return max(di.z - length(di), 0);
}

vec3 nSphere( in vec3 pos, in vec4 sph )
{
    return (pos-sph.xyz)/sph.w;
}

float calcSin(float minV,float distV){
    return ((sin(time) + 1) / 2 * distV) + minV;
}

float calcCos(float minV,float distV){
    return ((cos(time) + 1) / 2 * distV) + minV;
}

vec2 transform(vec2 pos, vec2 to){

    mat3 tran = edenMat2d;

    tran[2][0] = to.x;
    tran[2][1] = to.y;

    vec3 t = tran * vec3(pos, 1.0);

    return t.xy;
}

mat3 rotateX(float theta) {
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
    float c = cos(theta);
    float s = sin(theta);
    return mat3(
        vec3(c, -s, 0),
        vec3(s, c, 0),
        vec3(0, 0, 1)
    );
}

mat2 rotate2D(float theta) {
    float c = cos(theta);
    float s = sin(theta);
    return mat2(
        vec2(c, -s),
        vec2(s, c)
    );
}

vec2 rotate2D(vec2 pos, float theta) {
    float c = cos(theta);
    float s = sin(theta);
    return mat2(
        vec2(c, -s),
        vec2(s, c)
    ) * pos;
}

vec2 translate2D(vec2 samplePosition, vec2 offset){
    return samplePosition - offset;
}

mat3 rotate(vec3 rotate){
    mat3 rot = rotateX(rotate.x) * rotateY(rotate.y) * rotateZ(rotate.z);

    return rot;
}

float udSegment( in vec2 p, in vec2 a, in vec2 b )
{
    vec2 ba = b-a;
    vec2 pa = p-a;
    float h =clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
    return length(pa-h*ba);
}

float sdBox( vec3 p, vec3 b, vec3 off, mat3 transform)
{
    p = (p - off) * transform;
    vec3 q = abs(p) - b;
    float d =  length(max(q, 0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
    return d;
}

float circle(vec2 orig, vec2 off, float radius){
    vec2 com = orig - off;
    //get distance from center and grow it according to radius
    return length(com) - radius;
}

void mirror(inout vec2 position){
    position.x = abs(position.x);
}

float rectangle(vec2 org, vec2 off, vec2 halfSize, mat2 transform){
    vec2 com = org * transform;
    com = com - off;
    vec2 q = abs(com) - halfSize;
    float outsideDistance = length(max(q, 0));
    float insideDistance = min(max(q.x, q.y), 0);
    return outsideDistance + insideDistance;
}

float rectangle(vec2 org, vec2 halfSize){
    vec2 com = org;
    vec2 q = abs(com) - halfSize;
    float outsideDistance = length(max(q, 0));
    float insideDistance = min(max(q.x, q.y), 0);
    return outsideDistance + insideDistance;
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

vec2 cells(inout vec2 position, vec2 period){
    position = mod(position, period);
    //negative positions lead to negative modulo
    position += period;
    //negative positions now have correct cell coordinates, positive input positions too high
    position = mod(position, period);
    //second mod doesn't change values between 0 and period, but brings down values that are above period.

    vec2 cellIndex = position / period;
    cellIndex = floor(cellIndex);
    return cellIndex;
}

void wobble(inout vec2 position, vec2 frequency, vec2 amount){
    vec2 wobble = sin(position.yx * frequency) * amount;
    position = position + wobble;
}

void radial_cells(inout vec2 position, float cells){
    float cellSize = PI * 2 / cells;
    vec2 radialPosition = vec2(atan(position.x, position.y), length(position));
    radialPosition.x = mod(mod(radialPosition.x, cellSize) + cellSize, cellSize);

    position.x = sin(radialPosition.x) - cos(radialPosition.x);
    position.y = cos(radialPosition.x) + sin(radialPosition.x);

    position = position * radialPosition.y;
}


const int MAX_MARCHING_STEPS = 255;
const float MIN_DIST = 0.0;
const float MAX_DIST = 100.0;
const float PRECISION = 0.001;

float sdSphere(vec3 p, float r )
{
  vec3 offset = vec3(0, 0, -2);
  return length(p - offset) - r;
}

float rayMarch(vec3 ro, vec3 rd, float start, float end) {
  float depth = start;

  for (int i = 0; i < MAX_MARCHING_STEPS; i++) {
    vec3 p = ro + depth * rd;
    float d = sdSphere(p, 1.);
    depth += d;
    if (d < PRECISION || depth > end) break;
  }

  return depth;
}

mat3 camera(vec3 cameraPos, vec3 to) {
        vec3 cd = normalize(to - cameraPos);
        vec3 cr = normalize(cross(vec3(0, 1, 0), cd));
        vec3 cu = cross(cd, cr);

        return mat3(-cr, cu, -cd);
}

mat2 rotate2d(float theta) {
  float s = sin(theta), c = cos(theta);
  return mat2(c, -s, s, c);
}

vec3 calcNormal(vec3 p) {
    vec2 e = vec2(1.0, -1.0) * 0.0005; // epsilon
    float r = 1.; // radius of sphere
    return normalize(
      vec3(e.x, e.y, e.y) * sdSphere(p + vec3(e.x, e.y, e.y), r) +
      vec3(e.y, e.y, e.x) * sdSphere(p + vec3(e.y, e.y, e.x), r) +
      vec3(e.y, e.x, e.y) * sdSphere(p + vec3(e.y, e.x, e.y), r) +
      vec3(e.x, e.x, e.x) * sdSphere(p + vec3(e.x, e.x, e.x), r));
}

#define CAMERA_FOV 1.1

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
        float obj = sphereSDF(coord, vec3(0.0), 1.0);
    obj = min(obj, sphereSDF(coord, vec3(-1.0, 2.0, 1.4), 0.5));
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

void main(void)
{

    //Getting camera properties from buffer A
    vec3 cameraPos = cPos;
    vec3 cameraRotation = rot;

        //Creating ray
    vec3 rayOrigin = -cameraPos;
    mat3 look = look_at3(cameraPos, cameraPos + cameraRotation, vec3(0,1,0)) ;
    vec3 rayDirection = normalize(look * uvToCastPlane(fragTexCoord));

    vec4 col = vec4(0.0);

    vec3 hitPoint = rayMarch(rayOrigin, rayDirection);
    float sdf = objectSDF(hitPoint);

    bool isObject = sdf < 0.01;
    if (isObject)
    {
        vec3 normal = objectNormal(hitPoint);
        col.rgb += normal;
    }
    else
    {

    }

    outColor = col;
}
