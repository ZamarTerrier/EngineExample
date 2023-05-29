#version 450

#define MAX_TEXTURES 20
#define MAX_LIGHTS 16
#define MAX_SHADOWS 4

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

layout(binding = 1) uniform TextureBubber {
    int multi_size;
    int num_textures;
    vec2 some_val;
    vec4 tex_colors[MAX_TEXTURES];
} tb;

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

layout(binding = 6) uniform sampler2D shadowMap;
layout(binding = 7) uniform samplerCube shadowMap2[2];
layout(binding = 8) uniform sampler2D shadowMap3[2];
layout(binding = 9) uniform usampler2D texture_map;
layout(binding = 10) uniform sampler2D diffuse[MAX_TEXTURES];

layout(location = 0) in vec2 fragTexCoord;
layout(location = 1) in vec4 fragPos;
layout(location = 2) in vec3 fragNormal;
layout(location = 3) in float fragDepth;
layout(location = 4) in vec4 inViewPos;
layout(location = 5) in vec4 inModel;

layout(location = 0) out vec4 outColor;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec3 color, float shadow)
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
    return (ambient + (shadow + 0.35) * diffuse);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 color, float shadow)
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
    return (ambient + (shadow + 0.35) * diffuse);
}

vec3 CalcSpotLightBase(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 color, float shadow)
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
    return (ambient + (shadow + 0.35) * diffuse);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec3 color, float shadow)
{
    vec3 LightToPixel = normalize(fragPos - light.position);
    vec3 s_dir = light.direction * -1;
    float SpotFactor = dot(LightToPixel, s_dir);

    if (SpotFactor > light.cutOff) {
        vec3 Color = CalcSpotLightBase(light, normal, fragPos, viewDir, color, shadow);
        return Color * (1.0 - (1.0 - SpotFactor) * 1.0/(1.0 - light.cutOff));
    }
    else {
        return vec3(0,0,0);
    }
}

float shadowMapSizeInv = 0.001;

float ShadowSpot(vec4 ClipSpace, vec3 normal, int indx)
{
    float shadow = 0.0;
    vec3 projCoords = ClipSpace.xyz / ClipSpace.w;
    vec2 shadowCoords = projCoords.xy * 0.5 + vec2(0.5, 0.5);

    vec2 off0 = vec2(-0.9f, -0.8f) * (shadowMapSizeInv);
    vec2 off1 = vec2(+0.8f, -0.9f) * (shadowMapSizeInv);
    vec2 off2 = vec2(+0.8f, +1.0f) * (shadowMapSizeInv);
    vec2 off3 = vec2(+1.0f, -0.9f) * (shadowMapSizeInv);

    vec2 off4 = vec2( 0.0f,  0.0f) * (shadowMapSizeInv);
    vec2 off5 = vec2(-0.5f, -0.25f) * (shadowMapSizeInv);
    vec2 off6 = vec2(+0.25f, -0.5f) * (shadowMapSizeInv);
    vec2 off7 = vec2(+0.5f, +0.5f) * (shadowMapSizeInv);

    float bias = max(0.05 * (1.0 - dot(normal, slb.spots[indx].direction)), 0.005);

    float radius = length(projCoords);

    float shadow0, shadow1, shadow2, shadow3, shadow4, shadow5, shadow6, shadow7;

    if(radius < 1.2 && projCoords.z < 1.0 && projCoords.z > -1.0)
    {

        shadow0 = (projCoords.z - bias < texture(shadowMap3[indx], shadowCoords + off0).r + 0.001) ? 1.0f : 0.0;
        shadow1 = (projCoords.z - bias < texture(shadowMap3[indx], shadowCoords + off1).r + 0.001) ? 1.0f : 0.0;
        shadow2 = (projCoords.z - bias < texture(shadowMap3[indx], shadowCoords + off2).r + 0.001) ? 1.0f : 0.0;
        shadow3 = (projCoords.z - bias < texture(shadowMap3[indx], shadowCoords + off3).r + 0.001) ? 1.0f : 0.0;

        shadow4 = (projCoords.z - bias < texture(shadowMap3[indx], shadowCoords + off4).r + 0.001) ? 1.0f : 0.0;
        shadow5 = (projCoords.z - bias < texture(shadowMap3[indx], shadowCoords + off5).r + 0.001) ? 1.0f : 0.0;
        shadow6 = (projCoords.z - bias < texture(shadowMap3[indx], shadowCoords + off6).r + 0.001) ? 1.0f : 0.0;
        shadow7 = (projCoords.z - bias < texture(shadowMap3[indx], shadowCoords + off7).r + 0.001) ? 1.0f : 0.0;

        shadow = (1.0f / 9.0f) * ((shadow0 + shadow1 + shadow2 + shadow3) + (shadow4 + shadow5 + shadow6 + shadow7));
    }

    return shadow;
}

float ShadowClac(vec4 ClipSpace, vec3 normal)
{
    vec3 projCoords = ClipSpace.xyz / ClipSpace.w;
    vec2 shadowCoords = projCoords.xy * 0.5 + vec2(0.5, 0.5);

    vec2 off0 = vec2(-0.9f, -0.8f) * (shadowMapSizeInv);
    vec2 off1 = vec2(+0.8f, -0.9f) * (shadowMapSizeInv);
    vec2 off2 = vec2(+0.8f, +1.0f) * (shadowMapSizeInv);
    vec2 off3 = vec2(+1.0f, -0.9f) * (shadowMapSizeInv);

    vec2 off4 = vec2( 0.0f,  0.0f) * (shadowMapSizeInv);
    vec2 off5 = vec2(-0.5f, -0.25f) * (shadowMapSizeInv);
    vec2 off6 = vec2(+0.25f, -0.5f) * (shadowMapSizeInv);
    vec2 off7 = vec2(+0.5f, +0.5f) * (shadowMapSizeInv);

    float bias = max(0.01 * (1.0 - dot(normal, -dlb.dir[0].direction)), 0.005);

    float shadow0, shadow1, shadow2, shadow3, shadow4, shadow5, shadow6, shadow7;

    shadow0 = (projCoords.z - bias < texture(shadowMap, shadowCoords + off0).r + 0.001) ? 1.0f : 0.0;
    shadow1 = (projCoords.z - bias < texture(shadowMap, shadowCoords + off1).r + 0.001) ? 1.0f : 0.0;
    shadow2 = (projCoords.z - bias < texture(shadowMap, shadowCoords + off2).r + 0.001) ? 1.0f : 0.0;
    shadow3 = (projCoords.z - bias < texture(shadowMap, shadowCoords + off3).r + 0.001) ? 1.0f : 0.0;

    shadow4 = (projCoords.z - bias < texture(shadowMap, shadowCoords + off4).r + 0.001) ? 1.0f : 0.0;
    shadow5 = (projCoords.z - bias < texture(shadowMap, shadowCoords + off5).r + 0.001) ? 1.0f : 0.0;
    shadow6 = (projCoords.z - bias < texture(shadowMap, shadowCoords + off6).r + 0.001) ? 1.0f : 0.0;
    shadow7 = (projCoords.z - bias < texture(shadowMap, shadowCoords + off7).r + 0.001) ? 1.0f : 0.0;

    float shadow = (1.0f / 9.0f) * ((shadow0 + shadow1 + shadow2 + shadow3) + (shadow4 + shadow5 + shadow6 + shadow7));

    if(projCoords.z > 1.0f)
        shadow = 1.0;

    return shadow;
}

#define EPSILON 0.15
#define SHADOW_OPACITY 0.2

vec3 sampleOffsetDirections[20] = vec3[]
        (
            vec3( 1, 1, 1), vec3( 1, -1, 1), vec3(-1, -1, 1), vec3(-1, 1, 1),
            vec3( 1, 1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1, 1, -1),
            vec3( 1, 1, 0), vec3( 1, -1, 0), vec3(-1, -1, 0), vec3(-1, 1, 0),
            vec3( 1, 0, 1), vec3( -1, 0, 1), vec3( 1, 0, -1), vec3(-1, 0, -1),
            vec3( 0, 1, 1), vec3( 0, -1, 1), vec3(0, -1, -1), vec3( 0, 1, -1)
        );

float ShadwOmniCalc(vec3 somePos, int indx)
{
    float shadow = 0.0;
    float bias = 0.15;
    int samples = 20;
    vec3 fragToLight = somePos - plb.pos[indx].light_pos;

    float currentDepth = length(fragToLight);
    float viewDistance = length((inViewPos / inViewPos.w).xyz - somePos);
    float diskRadius = (1.0 + (viewDistance / 100)) / 25.0;
    for(int i=0;i < samples; i++)
    {
        float closestDepth = texture(shadowMap2[indx], fragToLight + sampleOffsetDirections[i] * diskRadius).r;
        closestDepth *= 100;
        if(currentDepth < closestDepth + bias)
            shadow +=1.0;
    }

    shadow /= float(samples);

    return shadow;
}

vec3 FindNearPixel(float cmp, inout int find, vec2 tiled_coords)
{
    vec3 mak_color = vec3(0);

    for(int i=0; i < tb.num_textures;i++)
    {
        float cmp_t = floor(tb.tex_colors[i].x) + floor(tb.tex_colors[i].y) + floor(tb.tex_colors[i].z) + floor(tb.tex_colors[i].w);

        if(cmp_t == cmp)
        {
            mak_color = texture(diffuse[i], tiled_coords).rgb;

            find = 1;

            break;
        }
    }

    return mak_color;
}

vec3 FindInQuality(float Quality, vec2 tiled_coords)
{
    vec3 color = vec3(0);

    float Pi = 3.1415926536;

    int near = 0;

    float cmp_temp = 0;

    ivec2 tex_size_vec = textureSize(texture_map, 0);
    float pixel_size_x = 1.0 / float(tex_size_vec.x);
    float pixel_size_y = 1.0 / float(tex_size_vec.y);

    for(float x= -pixel_size_x * Quality; x < pixel_size_x * Quality; x += pixel_size_x)
    {
        for(float y= -pixel_size_y * Quality; y < pixel_size_y * Quality; y += pixel_size_y)
        {
            vec4 temp_map = texture(texture_map, fragTexCoord + vec2(x, y));

            cmp_temp = floor(temp_map.x) + floor(temp_map.y) + floor(temp_map.z) + floor(temp_map.w);

            color = FindNearPixel(cmp_temp, near, tiled_coords);

            if(near == 1)
                break;
        }
    }

    return color;
}

vec4 FindDepthColor(inout float shadow)
{
    vec4 color = vec4(1);

    vec2 tiled_coords = fragTexCoord * tb.multi_size;

    vec2 tiled_coords2 = fragTexCoord * (tb.multi_size / 10);

    vec2 tiled_coords3 = fragTexCoord * (tb.multi_size / 25);

    float Quality = 4.0;

    if(fragDepth > 240)
    {
        float s_val = 1.0f / 240 * (fragDepth - 240);
        float s_val2 = 1.0f - s_val;

        vec3 color1 = FindInQuality(Quality, tiled_coords3) * s_val2;
        vec3 color2 = color.rgb * s_val;

        color = vec4(color1 + color2, 1.0);

        shadow += 0.35;

    }else if(fragDepth > 140)
    {
        float s_val = 1.0f / 240 * fragDepth;
        float s_val2 = 1.0f - s_val;

        vec3 color1 = FindInQuality(Quality, tiled_coords2) * s_val2;
        vec3 color2 = FindInQuality(Quality, tiled_coords3) * s_val;

        color = vec4(color1 + color2, 1.0);

        shadow += 0.25;

    }else if(fragDepth > 20)
    {
        float s_val = 1.0f / 140 * fragDepth;
        float s_val2 = 1.0f - s_val;

        vec3 color1 = FindInQuality(Quality, tiled_coords) * s_val2;
        vec3 color2 = FindInQuality(Quality, tiled_coords2) * s_val;

        color = vec4(color1 + color2, 1.0);

        shadow += 0.15;

    }else
        color = vec4(FindInQuality(Quality, tiled_coords), 1.0);

    return color;
}

void main() {

    vec4 color = vec4( 0.7, 0.7, 0.7, 1);

    float shadow = 0.0;

    if(lsb.num_dirs > 0)
    {

        vec4 fragClipSpace = dlb.mats[0].proj * dlb.mats[0].view * inModel;

        shadow += ShadowClac(fragClipSpace, fragNormal);
    }

    for(int i =0;i < lsb.num_points;i++)
        shadow += ShadwOmniCalc((inModel / inModel.w).xyz, i) / 2;

    for(int i =0;i < lsb.num_spots;i++){
        vec4 spotClipSpace = slb.mats[i].proj * slb.mats[i].view * inModel;

        shadow += ShadowSpot(spotClipSpace, fragNormal, i);
    }

    vec3 some_vec = (fragPos / fragPos.w).xyz;

    vec3 normal = vec3(0, 1, 0); //normalize(fragNormal);

    float some_dot = dot(vec3(0, 1, 0), dlb.dir[0].direction);

    if(some_dot > 0)
        shadow = 1.0;

    if(fragDepth < 480)
    {
        color = FindDepthColor(shadow);
    }else
        shadow = 1.0;

    if(shadow > 1.0)
        shadow = 1.0;


    if(lsb.isEnable != 0)
    {

        vec3 fullColor = vec3(0);

        for(int i =0;i < lsb.num_dirs;i++){
            DirLight d_light = dlb.dir[i];

            fullColor += CalcDirLight(d_light, normal, some_vec, color.xyz, shadow);

        }

        for(int i =0;i < lsb.num_points;i++){
            PointLight light = plb.points[i];

            vec4 L = normalize (vec4(light.position, 1.0) - fragPos);

            fullColor += CalcPointLight(light, normal, some_vec, L.xyz, color.xyz, shadow);
        }

        for(int i =0;i < lsb.num_spots;i++){
            SpotLight light = slb.spots[i];

            vec4 L = normalize (vec4(light.position, 1.0) - fragPos);

            fullColor += CalcSpotLight(light, normal, some_vec, L.xyz, color.xyz, shadow);
        }

        color = vec4(fullColor, color.a);
    }

    color.rgb *= shadow + 0.20;

    outColor = color;

}
