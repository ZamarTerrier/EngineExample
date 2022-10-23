#version 450

float PI = 3.14159265359;

#define MAX_MASKS_OBJECTS 256

struct ObjectParams{
    vec2 position;
    vec2 size;
};

layout(binding = 0) uniform GUIBuffer{
    vec2 mouse;
    vec2 position;
    vec2 size;
    vec3 color;
} guib;

layout(binding = 1) uniform MaskObjectsBuffer{
    ObjectParams objs[MAX_MASKS_OBJECTS];
    int size;
} mob;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

float sdBox( vec2 p, vec2 off,  vec2 b )
{
    vec2 org = p - (off + b);
    vec2 d = abs(org)-b;
    float outsideDistance = length(max(d, 0));
    float insideDistance = min(max(d.x, d.y), 0);
    return outsideDistance + insideDistance;
}

float guiScene(vec2 center){
    return sdBox(center, guib.position, guib.size);
}

void main(void)
{

    //Getting camera properties from buffer A

   float d = guiScene(fragTexCoord);

   vec3 color = vec3(0);

   if(d > 0)
   {
        discard;
   }
   else
   {
        for(int i=0; i < mob.size;i++){
           float d2 = sdBox(fragTexCoord, mob.objs[i].position, mob.objs[i].size);
           if(d2 > 0)
               discard;
       }

       color = guib.color;
   }

    outColor = vec4(color, 1.0);
}
