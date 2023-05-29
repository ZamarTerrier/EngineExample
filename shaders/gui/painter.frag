#version 450

float PI = 3.14159265359;

#define MAX_DRAW_OBJECTS 256

struct PObjectParams{
    vec2 position;
    vec2 size;
    int type;
    float angle;
    float radius;
    float transparent;
    vec4 color;
};

layout(binding = 0) uniform PainterBuffer{
    vec2 offset;
    vec2 position;
    vec2 size;
} pb;

layout(binding = 1) uniform DrawObjectsBuffer{
    PObjectParams objs[MAX_DRAW_OBJECTS];
    int size;
} dob;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

float sdBox( vec2 p, vec2 off,  vec2 b)
{
    vec2 org = p - (off + b);
    vec2 d = abs(org)-b;
    float outsideDistance = length(max(d, 0));
    float insideDistance = min(max(d.x, d.y), 0);
    return outsideDistance + insideDistance;
}

float sdCircle( vec2 p, vec2 off, float r )
{
    vec2 org = p - off;
    return length(org) - r;
}

float sdOrientedBox( in vec2 p, in vec2 off, in vec2 b, float th )
{
    float l = length(b-off);
    vec2  d = (b-off)/l;
    vec2  q = (p-(off+b)*0.5);
          q = mat2(d.x,-d.y,d.y,d.x)*q;
          q = abs(q)-vec2(l,th)*0.5;
    return length(max(q,0.0)) + min(max(q.x,q.y),0.0);
}

float sdSegment( in vec2 p, in vec2 a, in vec2 b )
{
    vec2 pa = p-a, ba = b-a;
    float h = clamp( dot(pa,ba)/dot(ba,ba), 0.0, 1.0 );
    return length( pa - ba*h );
}


void main(void)
{

    //Getting camera properties from buffer

    float d = 0, d2 = 0;

    vec4 color = vec4(0);

    for(int i=0; i < dob.size;i++){

        if(dob.objs[i].type == 0)
            d2 = sdBox(fragTexCoord, dob.objs[i].position, dob.objs[i].size);
        else if(dob.objs[i].type == 1)
            d2 = sdCircle(fragTexCoord, dob.objs[i].position, dob.objs[i].radius);
        else if(dob.objs[i].type == 2)
            d2 = sdOrientedBox(fragTexCoord, dob.objs[i].position, dob.objs[i].position + dob.objs[i].size, dob.objs[i].angle);
        else if(dob.objs[i].type == 3)
            d2 = sdSegment(fragTexCoord, dob.objs[i].position, dob.objs[i].size) - dob.objs[i].radius;

       if(d2 < d)
       {
           if(dob.objs[i].type == 3)
           {
               color.rgb = mix(dob.objs[i].color.rgb, color.rgb, step(0., d2 - 0.02));
           }else
                color = dob.objs[i].color;

           color.a = dob.objs[i].transparent;
           d = d2;
       }
    }

    if(d > 0)
        discard;

    outColor = color;
}
