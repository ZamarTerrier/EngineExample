#version 450

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


layout(binding = 2) uniform LightBufferObject{
    DirLight dir;
    PointLight lights[32];
    SpotLight spots[32];
    int num_points;
    int num_spots;
    int light_react;
} lbo;

layout(binding = 3) uniform sampler2D diffuse;
layout(binding = 4) uniform sampler2D specular;
layout(binding = 5) uniform sampler2D normalMap;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec4 fragPos;
layout(location = 3) in vec4 fragNormal;

layout(location = 0) out vec4 outColor;


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = max(dot(viewDir, reflectDir), 0.0);
    // combine results
    vec3 ambient  = light.ambient * vec3(texture(diffuse, fragTexCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(diffuse, fragTexCoord));
    vec3 specular = light.specular * spec * vec3(texture(specular, fragTexCoord));
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
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
    vec3 ambient  = light.ambient * vec3(texture(diffuse, fragTexCoord));
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(diffuse, fragTexCoord));
    vec3 specular = light.specular * spec * vec3(texture(specular, fragTexCoord));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLightBase(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
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
    vec3 ambient  = light.ambient * vec3(texture(diffuse, fragTexCoord));;
    vec3 diffuse  = light.diffuse  * diff * vec3(texture(diffuse, fragTexCoord));
    vec3 specular = light.specular * spec * vec3(texture(specular, fragTexCoord));
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 LightToPixel = normalize(fragPos - light.position);
    float SpotFactor = dot(LightToPixel, light.direction);

    if (SpotFactor > light.cutOff) {
        vec3 Color = CalcSpotLightBase(light, normal, fragPos, viewDir);
        return Color * (1.0 - (1.0 - SpotFactor) * 1.0/(1.0 - light.cutOff));
    }
    else {
        return vec3(0,0,0);
    }
}

void main() {

    vec4 color = texture(diffuse, fragTexCoord);

    if(color.a < 0.5f)
        discard;

    if(lbo.light_react != 0)
    {
        vec3 fullColor = vec3(0);

        vec3 normal = texture(normalMap, fragTexCoord).xyz;

        if(normal.x == 0 && normal.y == 0 && normal.z == 0)
            normal = normalize(fragNormal.xyz);
        else
            // transform normal vector to range [-1,1]
            normal = normalize(normal * 2.0 - 1.0);

        fullColor += CalcDirLight(lbo.dir, normal, fragPos.xyz);

        for(int i =0;i < lbo.num_points;i++){
            PointLight light = lbo.lights[i];

            vec4 L = normalize (vec4(light.position, 1.0) - fragPos);

            fullColor += CalcPointLight(light, normal, fragPos.xyz, L.xyz);
        }

        for(int i =0;i < lbo.num_spots;i++){
            SpotLight light = lbo.spots[i];

            vec4 L = normalize (vec4(light.position, 1.0) - fragPos);

            fullColor += CalcSpotLight(light, normal, fragPos.xyz, L.xyz);
        }

        if(lbo.num_points == 0 && lbo.num_spots == 0)
            outColor = clamp(texture(specular, fragTexCoord) + color, vec4(0), vec4(1));
        else
            outColor = vec4(fullColor, 1.0);
    }
    else
        outColor = clamp(texture(specular, fragTexCoord) + color, vec4(0), vec4(1));
}
