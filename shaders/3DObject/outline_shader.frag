#version 450

layout(location = 0) in vec4 fragVerPos;

layout(location = 0) out vec4 outColor;

vec3 LineColor = vec3(0.5, 0.5, 0.5);

vec2 resolution = vec2(1024, 800);

float circleShape(float radius, vec2 position){
    float value = distance(position, vec2(0.5));
    return step(radius, value);
}

void main(void)
{
    vec2 coord = outColor.xy / resolution;
    float circleWidth = 0.2;
    float circle = circleShape(circleWidth, coord);
    vec3 color = vec3(circle);

    if(circle == 0.)
        outColor = vec4(LineColor, 1.0);
}
