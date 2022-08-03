#version 450

#define pi 3.14159265

#define WIDTH 800
#define HEIGHT 600

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in float time;

vec2 dotPos = vec2(0.5, 0.5);
vec2 viewOffset = vec2(0,0);
vec2 viewSize = vec2(WIDTH, HEIGHT);

vec3 camPos = vec3(0, 0, -5);
vec3 camRot = vec3(0, 0, 0);

vec3 spherePos = vec3(0.3, 0.3, 0.3);

mat4 edenMat = mat4(
    vec4(1,0,0,0),
    vec4(0,1,0,0),
    vec4(0,0,1,0),
    vec4(0,0,0,1)
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

float drawSphere(vec4 p, float d, vec2 uv)
{
    return (distance(p.xy, uv) <= d) ? 1.0 : 0.0;
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

    mat4 result = mat4(vec4(x.x, x.y, x.z, -dot(from, x)),
                       vec4(y.x, y.y, y.z, -dot(from, y)),
                       vec4(z.x, z.y, z.z, -dot(from, z)),
                       vec4(0,   0,   0,    1));
        /*vec4(x.x, x.y, x.z, -dot(from, x)),
        vec4(y.x, y.y, y.z, -dot(from, y)),
        vec4(z.x, z.y, z.z, -dot(from, z)),
        vec4(0,   0,   0,    1));*/

    return result;
}


mat4 m4_rotation_matrix(mat4 matrix, vec3 degrees)
{

  degrees.x *= pi / 180;  // convert to radians
  degrees.y *= pi / 180;
  degrees.z *= pi / 180;

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
  float tan_half_fov = tan((fov_degrees * (pi / 180)) / 2.0);

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

void main(void)
{

    vec2 linePos = calcLine();
    vec2 linePos2 = calcLine();
    linePos2.x +=0.1;

    float lines = drawLine(vec2(0.1, 0.1), linePos2, fragTexCoord, 1.0);


    float dots = drawDot(vec2(abs(sin(time)), 0.2), abs(sin(time)) / 8, fragTexCoord);

    vec3 color = vec3(0.5, 0.0, 0.0);

    //outColor = vec4(color, 1.0);

    //outColor = vec4(fragColor, 1.0);

    float bizare = 0;

    vec3 lastPoint = vec3(0.1, 0.1, 1.0);
    vec3 endPoint = vec3(0.5, 0.5, 1.0);

    for(int i=0; i < 10;i++){
        vec3 p = toBezier(0.1, i, lastPoint, vec3(abs(sin(time)) - 0.25, abs(cos(time)) - 0.25, 1.0), endPoint);

        bizare += drawLine(lastPoint.xy, p.xy, fragTexCoord, 1.0);

        lastPoint = p;
    }

    bizare += drawLine(lastPoint.xy, endPoint.xy, fragTexCoord, 1.0);

    mat4 perspec = m4_perspective(45, 0.1, 100);
    mat4 view = look_at(camPos, camPos + camRot, vec3(0, 1, 0));
    mat4 model = m4_translate(vec3(spherePos)) * m4_rotation_matrix(edenMat, vec3(0, 0, 0)) * m4_scale(vec3(1, 1, 1));

    vec4 sphereOrg = perspec * view * vec4(0, 0, 0, 1.0); //perspec * view * model * vec4(0,0,0,1.0);

    float sphere = drawSphere(sphereOrg, 0.1, fragTexCoord);

    if(sphere > 0)
        color = vec3(0.2, 0.5, 0.0);
    else if (bizare > 0)
        color = vec3(0.0, 0.5, 0.0);
    else if(dots > 0)
        color = vec3(0.0, 0.4, 0.2);
    else if(lines > 0)
        color = vec3(0.0, 0.0, 0.7);

    outColor = vec4(color, 1.0);
}
