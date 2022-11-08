#include "sprites.h"

#include "engine.h"

#include "spriteObject.h"

#include "resource.h"

#include "math.h"

SpriteObject sprite;

float x = 0, y = 0;
float o_x = 0, o_y = 50;
float time;

bool right = false;
bool stay = true;

void SpriteInit()
{
    SpriteParam sParam;
    ToolsAddStrings(sParam.texturePath, 256, path, "/textures/super_hero.png");
    SpriteObjectInit(&sprite, sParam);

    Transform2DSetImageScale(&sprite, 48, 48);
    Transform2DSetImageOffset(&sprite, o_x, o_y);

}

void SpriteUpdate()
{
    time += 0.1f;

    if(time >= 5)
    {
        if(stay && !right)
        {
            o_x = 100;
            right = true;
            stay = false;
        }
        else if(stay && right)
        {
            o_x = 0;
            right = false;
            stay = false;
        }
        else
        {
            o_x = 50;
            stay = true;
        }

        Transform2DSetImageOffset(&sprite, o_x, o_y);

        time = 0;
    }
}

void SpriteDraw()
{
    engDraw(&sprite);
}


void SpriteDestroy()
{
    GameObjectDestroy(&sprite);
}
