#version 450

layout(binding = 1) uniform WaterBuffer {
    float time;
} wb;

layout(binding = 2) uniform sampler2D diffuse;

layout(location = 0) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

float fog(float density)
{
        const float LOG2 = -1.442695;
        float dist = gl_FragCoord.z / gl_FragCoord.w * 0.0015;
        float d = density * dist;
        return 1.0 - clamp(exp2(d * d * LOG2), 0.0, 1.0);
}

void main() {

    float s_val = (sin(wb.time) + 1) / 160;
    float c_val = (cos(wb.time) + 1) / 160;

    vec2 offset = vec2(s_val, c_val);

    float time = wb.time / 200;

    vec4 color = texture(diffuse, fragTexCoord + offset + time) / 40;

    color.a = 0.95;

    const vec4 fogColor = vec4(0.47, 0.5, 0.67, 0.0);
    outColor = mix(color, fogColor, fog(0.25));

}
