#version 450

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

layout(binding = 6) uniform sampler2D shadowMap[MAX_SHADOWS];
layout(binding = 7) uniform samplerCube shadowMap2[2];
layout(binding = 8) uniform sampler2D shadowMap3[2];
layout(binding = 9) uniform sampler2D diffuse;
layout(binding = 10) uniform sampler2D normalMap;

layout(location = 0) in vec2 fragTexCoord;
layout(location = 1) in vec3 fragNormal;
layout(location = 2) in vec4 fragPos;
layout(location = 3) in vec4 inModel;
layout(location = 4) in float fragDepth;
layout(location = 5) in vec3 inWorldPos;
layout(location = 6) in vec4 inViewPos;

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
    vec3 ambient  = light.ambient * color; //vec3(texture(diffuse, fragTexCoord));
    vec3 diffuse  = light.diffuse  * diff * color; //vec3(texture(diffuse, fragTexCoord));
    return (ambient + (shadow + 0.25) * diffuse);
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
    vec3 ambient  = light.ambient * color; //vec3(texture(diffuse, fragTexCoord));
    vec3 diffuse  = light.diffuse  * diff * color; //vec3(texture(diffuse, fragTexCoord));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    return (ambient + (shadow + 0.25) * diffuse);
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
    vec3 ambient  = light.ambient * color; //vec3(texture(diffuse, fragTexCoord));;
    vec3 diffuse  = light.diffuse  * diff * color; //vec3(texture(diffuse, fragTexCoord));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    return (ambient + (shadow + 0.25) * diffuse);
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

float shadowMapSizeInv = 0.0001;

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

float ShadowClac(vec4 ClipSpace, int shadow_indx, vec3 normal)
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

    float bias = max(0.05 * (1.0 - dot(normal, -dlb.dir[0].direction)), 0.005);

    float shadow0, shadow1, shadow2, shadow3, shadow4, shadow5, shadow6, shadow7;

    shadow0 = (projCoords.z - bias < texture(shadowMap[shadow_indx], shadowCoords + off0).r + 0.001) ? 1.0f : 0.0;
    shadow1 = (projCoords.z - bias < texture(shadowMap[shadow_indx], shadowCoords + off1).r + 0.001) ? 1.0f : 0.0;
    shadow2 = (projCoords.z - bias < texture(shadowMap[shadow_indx], shadowCoords + off2).r + 0.001) ? 1.0f : 0.0;
    shadow3 = (projCoords.z - bias < texture(shadowMap[shadow_indx], shadowCoords + off3).r + 0.001) ? 1.0f : 0.0;

    shadow4 = (projCoords.z - bias < texture(shadowMap[shadow_indx], shadowCoords + off4).r + 0.001) ? 1.0f : 0.0;
    shadow5 = (projCoords.z - bias < texture(shadowMap[shadow_indx], shadowCoords + off5).r + 0.001) ? 1.0f : 0.0;
    shadow6 = (projCoords.z - bias < texture(shadowMap[shadow_indx], shadowCoords + off6).r + 0.001) ? 1.0f : 0.0;
    shadow7 = (projCoords.z - bias < texture(shadowMap[shadow_indx], shadowCoords + off7).r + 0.001) ? 1.0f : 0.0;

    float shadow = (1.0f / 9.0f) * ((shadow0 + shadow1 + shadow2 + shadow3) + (shadow4 + shadow5 + shadow6 + shadow7));

    if(projCoords.z > 1.0f)
        shadow = 0.0;

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

void main() {

    vec4 color = texture(diffuse, fragTexCoord);

    if(color.a < 0.9)
        discard;

    vec4 resColor = vec4(1);

    vec3 normal = texture(normalMap, fragTexCoord).xyz;

    float shadow = 0.0;

    if(lsb.num_dirs > 0)
    {
        int cascadeIndex = 0;

        for(int i=0;i< MAX_SHADOWS - 1;i++)
        {
            if(inViewPos.z < dlb.cascadeSplits[i])
                cascadeIndex = i + 1;
        }

        vec4 fragClipSpace = dlb.mats[cascadeIndex].proj * dlb.mats[cascadeIndex].view * inModel;

        shadow += ShadowClac(fragClipSpace, cascadeIndex, fragNormal);
    }


    for(int i =0;i < lsb.num_points;i++)
        shadow += ShadwOmniCalc((inModel / inModel.w).xyz, i);

    for(int i =0;i < lsb.num_spots;i++){
        vec4 spotClipSpace = slb.mats[i].proj * slb.mats[i].view * inModel;

        shadow += ShadowSpot(spotClipSpace, fragNormal, i);
    }

    vec3 some_vec = (fragPos / fragPos.w).xyz;

    if(lsb.isEnable != 0)
    {
        vec3 fullColor = vec3(0);

        vec3 normal = texture(normalMap, fragTexCoord).rgb;

        for(int i =0;i < lsb.num_dirs;i++){
            DirLight d_light = dlb.dir[i];

            fullColor += CalcDirLight(d_light, fragNormal, some_vec, color.xyz, shadow);

        }

        for(int i =0;i < lsb.num_points;i++){
            PointLight light = plb.points[i];

            vec4 L = normalize (vec4(light.position, 1.0) - fragPos);

            fullColor += CalcPointLight(light, fragNormal, some_vec, L.xyz, color.xyz, shadow);
        }

        for(int i =0;i < lsb.num_spots;i++){
            SpotLight light = slb.spots[i];

            vec4 L = normalize (vec4(light.position, 1.0) - fragPos);

            fullColor += CalcSpotLight(light, fragNormal, some_vec, L.xyz, color.xyz, shadow);
        }

        resColor = vec4(fullColor, color.a);
    }
    else
        resColor = color;

    resColor.rgb *= shadow + 0.20; //+ 0.35 > 1.0 ? 1.0 : shadow + 0.35;

    outColor = resColor;
}
