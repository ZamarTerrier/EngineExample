#version 450

layout(binding = 0) uniform ViewBuffer3D {
    vec3 position;
    vec3 rotation;
    vec3 scale;
} vbo;

layout(binding = 1) uniform TransformBuffer3D {
    vec3 position;
    vec3 rotation;
    vec3 scale;
} tbo;

layout(binding = 2) uniform ModelBuffer3D{
    mat4 model;
    mat4 view;
    mat4 proj;
} mbo;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 fragColor;
layout(location = 1) out vec2 fragTexCoord;
layout(location = 2) out vec4 fragVertCoord;

float PI = 3.14159265358979323846;

mat4 edenMat = mat4(
    vec4(1,0,0,0),
    vec4(0,1,0,0),
    vec4(0,0,1,0),
    vec4(0,0,0,1)
);

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

void main() {

    //scale
    mat4 scale_mat = edenMat;

    scale_mat[0][0] = tbo.scale.x * 1.1;
    scale_mat[1][1] = tbo.scale.y * 1.1;
    scale_mat[2][2] = tbo.scale.z * 1.1;

    //rotate
    mat4 rot_mat = m4_rotation_matrix(edenMat, tbo.rotation);

    mat4 trans_mat = edenMat;
    //translate
    trans_mat[3][0] += tbo.position.x;
    trans_mat[3][1] += tbo.position.y;
    trans_mat[3][2] += tbo.position.z;

    mat4 model = trans_mat * rot_mat * scale_mat;

    mat4 view = look_at(vbo.position, vbo.position + vbo.rotation, vec3(0.0, 1.0, 0.0));

    fragColor = color;
    fragTexCoord = inTexCoord;
    fragVertCoord =  mbo.proj * mbo.view * model * vec4(position, 1.0);
    gl_Position = fragVertCoord;

}
