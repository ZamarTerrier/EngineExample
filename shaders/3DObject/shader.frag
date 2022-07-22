#version 450

layout(binding = 3) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

float outline_thickness = 0.2;
vec3 outline_colour = vec3(0, 0, 1);
float outline_threshold = 0.5;

layout(location = 0) out vec4 outColor;

void main() {

    vec4 color = texture(texSampler, fragTexCoord);

    if (color.a <= outline_threshold) {
            ivec2 size = textureSize(texSampler, 0);

            float uv_x = fragTexCoord.x * size.x;
            float uv_y = fragTexCoord.y * size.y;

            float sum = 0.0;
            for (int n = 0; n < 9; ++n) {
                uv_y = (fragTexCoord.y * size.y) + (outline_thickness * float(n - 4.5));
                float h_sum = 0.0;
                h_sum += texelFetch(texSampler, ivec2(uv_x - (4.0 * outline_thickness), uv_y), 0).a;
                h_sum += texelFetch(texSampler, ivec2(uv_x - (3.0 * outline_thickness), uv_y), 0).a;
                h_sum += texelFetch(texSampler, ivec2(uv_x - (2.0 * outline_thickness), uv_y), 0).a;
                h_sum += texelFetch(texSampler, ivec2(uv_x - outline_thickness, uv_y), 0).a;
                h_sum += texelFetch(texSampler, ivec2(uv_x, uv_y), 0).a;
                h_sum += texelFetch(texSampler, ivec2(uv_x + outline_thickness, uv_y), 0).a;
                h_sum += texelFetch(texSampler, ivec2(uv_x + (2.0 * outline_thickness), uv_y), 0).a;
                h_sum += texelFetch(texSampler, ivec2(uv_x + (3.0 * outline_thickness), uv_y), 0).a;
                h_sum += texelFetch(texSampler, ivec2(uv_x + (4.0 * outline_thickness), uv_y), 0).a;
                sum += h_sum / 9.0;
            }

            if (sum / 9.0 >= 0.0001) {
                color = vec4(outline_colour, 1);
            }
        }

    outColor = vec4(fragColor * color.rgb, 1.0);
}
