#include "sprites.h"

#include "engine.h"

#include "textObject.h"
#include "spriteObject.h"
#include "shapeObject.h"

#include "intersections2D.h"

#include "resource.h"

#include "math.h"

SpriteObject sprite;
ShapeObject obj1;
ShapeObject obj2;
ShapeObject obj3;

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


    DrawParam dParam;
    ToolsAddStrings(dParam.diffuse, 256, path, "/textures/texture.jpg");

    vec2 positions[] = {
        {20, 20},
        {200, 0},
        {250, 180},
        {180, 220},
        {0, 200},
    };

    QuadParams quad;
    quad.color = (vec3){1, 1, 1};
    quad.size = 100;


    CircleParams circle;
    circle.color = (vec3){1, 1, 1};
    circle.radius = 100;
    circle.freq = 20;

    ShapeParams shape;
    shape.positions = positions;
    shape.color = (vec3){1, 1, 1};
    shape.num_positions = 5;

    ShapeObjectInit(&obj1, dParam, ENGINE_SHAPE_OBJECT_CIRCLE, &circle);
    Transform2DSetPosition(&obj1, 300, 300);
    ShapeObjectInit(&obj2, dParam, ENGINE_SHAPE_OBJECT_QUAD, &quad);
    Transform2DSetPosition(&obj2, 300, 300);
    ShapeObjectInit(&obj3, dParam, ENGINE_SHAPE_OBJECT_SHAPE, &shape);
    Transform2DSetPosition(&obj3, 300, 300);

}

void AnimSprite(float deltaTime)
{
    time += 2 * deltaTime;

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

void SpriteUpdate(float deltaTime)
{
    AnimSprite(deltaTime);

    double mX, mY;

    EngineGetCursorPos(&mX, &mY);

    Transform2DSetPosition(&obj2, mX * 2, mY * 2);

    /*InterSquareParam param1;
    param1.position = Transform2DGetPosition(&obj1);
    param1.size = 100;*/


    InterCircleParam param1;
    param1.center = Transform2DGetPosition(&obj1);
    param1.radius = 100;


    InterSquareParam param2;
    param2.position = Transform2DGetPosition(&obj2);
    param2.size = 100;

    float dist, depth;
    vec2 dir;

    int res = IntersectionCircleSquare(&param1, &param2, &dist, &depth, &dir);

}

void SpriteDraw()
{
    engDraw(&sprite);

    engDraw(&obj1);
    engDraw(&obj2);
    engDraw(&obj3);
}


void SpriteDestroy()
{
    GameObjectDestroy(&sprite);

    GameObjectDestroy(&obj1);
    GameObjectDestroy(&obj2);
    GameObjectDestroy(&obj3);
}
