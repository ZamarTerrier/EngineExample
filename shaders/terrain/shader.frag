#version 450

#define MAX_TEXTURES 20
#define MAX_LIGHTS 16

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

layout(binding = 2) uniform TextureBubber {
    int multi_size;
} tb;

layout(binding = 3) uniform DirLightBuffer{
    DirLight dir[MAX_LIGHTS];
    LightMatrix mats[MAX_LIGHTS];
    vec4 cascadeSplits;
} dlb;

layout(binding = 4) uniform PointLightBuffer{
    PointLight points[MAX_LIGHTS];
    LightPos pos[MAX_LIGHTS];
} plb;

layout(binding = 5) uniform SpotLightBuffer{
    SpotLight spots[MAX_LIGHTS];
    LightMatrix mats[MAX_LIGHTS];
} slb;

layout(binding = 6) uniform LightStatusBuffer{
    int num_dirs;
    int num_points;
    int num_spots;
    int isEnable;
} lsb;

layout(binding = 7) uniform sampler2D heightMap;
layout(binding = 8) uniform usampler2D texture_map;
layout(binding = 9) uniform sampler2D diffuse[MAX_TEXTURES];

layout (location = 0) in vec3 inNormal;
layout (location = 1) in vec2 inUV;
layout (location = 2) in vec3 inViewVec;
layout (location = 3) in vec3 inLightVec;
layout (location = 4) in vec3 inEyePos;
layout (location = 5) in vec3 inWorldPos;
layout (location = 6) in float inDepth;

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
    vec3 ambient  = light.ambient * color;
    vec3 diffuse  = light.diffuse * diff * color;
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
    vec3 ambient  = light.ambient * color;
    vec3 diffuse  = light.diffuse  * diff * color;
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
    vec3 ambient  = light.ambient * color;
    vec3 diffuse  = light.diffuse  * diff * color;
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

vec4 FindDepthColor()
{
    vec4 color = vec4(1);

    float Quality = 4.0;

    vec2 tiled_coords = inUV * tb.multi_size;

    uint cmp_t;

    uint cmp = texture(texture_map, inUV).r;

    color.rgb = texture(diffuse[cmp], tiled_coords).rgb;

    /*
    for(int i=0; i < tb.num_textures;i++)
    {
        cmp_t = tb.tex_colors[i];

        if(cmp_t == cmp)
        {
            color.rgb = texture(diffuse[i], tiled_coords).rgb;

            break;
        }
    }*/

    return color;
}

float fog(float density)
{
        const float LOG2 = -1.442695;
        float dist = gl_FragCoord.z / gl_FragCoord.w * 0.0015;
        float d = density * dist;
        return 1.0 - clamp(exp2(d * d * LOG2), 0.0, 1.0);
}

void main() {

    vec4 color = vec4(0.5); // texture(diffuse[2], inUV);

    color = FindDepthColor();

    vec3 normal = vec3(0,1,0);// normalize(inNormal);

    vec3 some_vec = inWorldPos; //(fragPos / fragPos.w).xyz;

    if(lsb.isEnable != 0)
    {

        vec3 fullColor = vec3(0);

        for(int i =0;i < lsb.num_dirs;i++){
            DirLight d_light = dlb.dir[i];

            fullColor += CalcDirLight(d_light, normal, some_vec, color.xyz);

        }

        for(int i =0;i < lsb.num_points;i++){
            PointLight light = plb.points[i];

            vec4 L = normalize (vec4(light.position - inWorldPos, 1.0));

            fullColor += CalcPointLight(light, normal, some_vec, L.xyz, color.rgb);
        }

        for(int i =0;i < lsb.num_spots;i++){
            SpotLight light = slb.spots[i];

            vec4 L = normalize (vec4(light.position - inWorldPos, 1.0));

            fullColor += CalcSpotLight(light, normal, some_vec, L.xyz, color.rgb);
        }

        color = vec4(fullColor, 1.0);
    }

    const vec4 fogColor = vec4(0.47, 0.5, 0.67, 0.0);
    outColor = mix(color, fogColor, fog(0.25));
}
