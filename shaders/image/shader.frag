#version 450

#define MAX_TEXTURES 20

layout(binding = 0) uniform TextureBubber {
    int multi_size;
    int num_textures;
    vec2 some_val;
    vec4 tex_colors[MAX_TEXTURES];
} tb;

layout(binding = 1) uniform usampler2D texture_map;
layout(binding = 2) uniform sampler2D diffuse[MAX_TEXTURES];

layout(location = 0) in vec2 fragTexCoord;
layout(location = 1) in float fragDepth;

layout(location = 0) out vec4 outColor;

void main() {

    vec4 color = vec4( 0.7, 0.7, 0.7, 1);

    vec4 map = texture(texture_map, fragTexCoord);

    float cmp_1 = 0, cmp_2 = map.x + map.y + map.z + map.w;

    for(int i=0; i < tb.num_textures;i++)
    {
        cmp_1 = floor(tb.tex_colors[i].x) + floor(tb.tex_colors[i].y) + floor(tb.tex_colors[i].z) + floor(tb.tex_colors[i].w);

        if(cmp_1 == cmp_2)
        {

            vec2 tiled_coords = fragTexCoord * tb.multi_size;


            color = texture(diffuse[i], tiled_coords);

            break;
        }

    }

    outColor = color;

}
