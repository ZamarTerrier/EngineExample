#version 450

#define MAX_LIGHTS 16
#define MAX_LIGHT_MATRIX 16
#define MAX_SHADOWS 4

#define MAX_FRAGMENT_COUNT 128

struct Node
{
    vec4 color;
    float depth;
    uint next;
};

struct LightMatrix{
    mat4 proj;
    mat4 view;
};

struct PointLight{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct DirLight {
    vec3 position;
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight{
    vec3 position;
    vec3  direction;

    float constant;
    float linear;
    float quadratic;
    float cutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct LightPos{
    vec3 light_pos;
    vec3 view_pos;
};

layout(binding = 2) uniform DirLightBuffer{
    DirLight dir[MAX_LIGHTS];
    LightMatrix mats[MAX_LIGHTS];
    vec4 cascadeSplits;
} dlb;

layout(binding = 3) uniform PointLightBuffer{
    PointLight points[MAX_LIGHTS];
    LightPos pos[MAX_LIGHTS];
} plb;

layout(binding = 4) uniform SpotLightBuffer{
    SpotLight spots[MAX_LIGHTS];
    LightMatrix mats[MAX_LIGHTS];
} slb;

layout(binding = 5) uniform LightStatusBuffer{
    int num_dirs;
    int num_points;
    int num_spots;
    int isEnable;
} lsb;


layout(binding = 6) uniform sampler2D diffuse;
layout(binding = 7) uniform sampler2D normalMap;

layout(location = 0) in vec2 fragTexCoord;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec4 fragPos;
layout(location = 3) in vec4 inModel;
layout(location = 4) in float fragDepth;
layout(location = 5) in vec3 inWorldPos;
layout(location = 6) in vec4 inViewPos;

layout(location = 0) out vec4 outColor;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 color)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = max(dot(viewDir, reflectDir), 0.0);
    // combine results
    vec3 ambient  = light.ambient * color; //vec3(texture(diffuse, fragTexCoord));
    vec3 diffuse  = light.diffuse  * diff * color; //vec3(texture(diffuse, fragTexCoord));
    return (ambient + diffuse);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 color)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = max(dot(viewDir, reflectDir), 0.0);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
                             light.quadratic * (distance * distance));
    // combine results
    vec3 ambient  = light.ambient * color; //vec3(texture(diffuse, fragTexCoord));
    vec3 diffuse  = light.diffuse  * diff * color; //vec3(texture(diffuse, fragTexCoord));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    return (ambient + diffuse);
}

vec3 CalcSpotLightBase(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 color)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = max(dot(viewDir, reflectDir), 0.0);
    // attenuation
    float distance    = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance +
                             light.quadratic * (distance * distance));
    // combine results
    vec3 ambient  = light.ambient * color; //vec3(texture(diffuse, fragTexCoord));;
    vec3 diffuse  = light.diffuse  * diff * color; //vec3(texture(diffuse, fragTexCoord));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    return (ambient + diffuse);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 color)
{
    vec3 LightToPixel = normalize(fragPos - light.position);
    vec3 s_dir = light.direction * -1;
    float SpotFactor = dot(LightToPixel, s_dir);

    if (SpotFactor > light.cutOff) {
        vec3 Color = CalcSpotLightBase(light, normal, fragPos, viewDir, color);
        return Color * (1.0 - (1.0 - SpotFactor) * 1.0/(1.0 - light.cutOff));
    }
    else {
        return vec3(0,0,0);
    }
}

void main() {

    vec4 color = texture(diffuse, fragTexCoord);

    if(color.a < 0.9)
        discard;

    vec4 resColor = vec4(1);

    vec3 normal = texture(normalMap, fragTexCoord).xyz;

    vec3 some_vec = (fragPos / fragPos.w).xyz;

    if(lsb.isEnable != 0)
    {
        vec3 fullColor = vec3(0);

        for(int i =0;i < lsb.num_dirs;i++){
            DirLight d_light = dlb.dir[i];

            fullColor += CalcDirLight(d_light, fragNormal, some_vec, color.xyz);

        }

        for(int i =0;i < lsb.num_points;i++){
            PointLight light = plb.points[i];

            vec4 L = normalize (vec4(light.position, 1.0) - fragPos);

            fullColor += CalcPointLight(light, fragNormal, some_vec, L.xyz, color.xyz);
        }

        for(int i =0;i < lsb.num_spots;i++){
            SpotLight light = slb.spots[i];

            vec4 L = normalize (vec4(light.position, 1.0) - fragPos);

            fullColor += CalcSpotLight(light, fragNormal, some_vec, L.xyz, color.xyz);
        }

        resColor = vec4(fullColor, color.a);
    }
    else
        resColor = color;

    outColor = resColor;
}
