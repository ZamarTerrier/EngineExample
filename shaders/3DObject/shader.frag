#version 450

layout(binding = 3) uniform sampler2D texSampler;
layout(binding = 4) uniform sampler2D waveSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec3 fragPos;
layout(location = 3) in vec3 fragNormal;

vec3 lightPos = vec3(0,0,9.5);

layout(location = 0) out vec4 outColor;

void main() {

    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * fragColor;

    vec3 norm = normalize(fragNormal);
    vec3 lightdir = normalize(lightPos - fragPos);

    float diff = max(dot(norm, lightdir), 0.0);
    vec3 diffuse = diff * fragColor;

    // specular
    float specularStrength = 0.5;
    vec3 viewDir = normalize(-fragPos); // the viewer is always at (0,0,0) in view-space, so viewDir is (0,0,0) - Position => -Position
    vec3 reflectDir = reflect(-lightdir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * fragColor;

    vec4 color = texture(texSampler, fragTexCoord);
    vec3 result = (ambient + diffuse + specular) * color.rgb;

    outColor = vec4(result, 1.0);
}
