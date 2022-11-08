#version 450

#define MAX_LIGHT 256

struct Light{
    vec3 position;
    vec3 color;
};

layout(binding = 1) uniform LightBufferObject{
    Light lights[256];
    int size;
} lbo;

layout(binding = 2) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;
layout(location = 2) in vec4 fragPos;
layout(location = 3) in vec4 fragNormal;

vec3 lightPos = vec3(0,0,9.5);

layout(location = 0) out vec4 outColor;

void main() {

    vec4 color = texture(texSampler, fragTexCoord);

<<<<<<< HEAD
    if(color.a < 0.5f)
        discard;

    vec3 fullColor = color.rgb * fragColor;
=======
    vec3 fullColor = color.rgb + fragColor;
>>>>>>> origin/master

    for(int i =0;i < lbo.size;i++){
        Light light = lbo.lights[i];

        vec4 N= normalize(fragNormal);

        vec4 L = normalize (vec4(light.position, 1.0) - fragPos);

        float NL = max(dot(N,L), 0);

        fullColor *= NL * light.color;
    }


    outColor = vec4(fullColor, 1.0);
}
