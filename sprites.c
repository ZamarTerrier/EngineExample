#include "sprites.h"

#include <wchar.h>
#include <stdlib.h>

#include "engine.h"

#include "textObject.h"
#include "spriteObject.h"
#include "shapeObject.h"

#include "intersections2D.h"
#include "rayIntersections2D.h"

#include "particleSystem2D.h"

#include "resource.h"

#include "math.h"

#include "e_resource_data.h"

SpriteObject sprite;
ShapeObject obj1;
ShapeObject obj2;
ShapeObject obj3;

TextObject interText;

float x = 0, y = 0;
float o_x = 0, o_y = 50;
float time;

bool right = false;
bool stay = true;

void SpriteInit()
{
    DrawParam dParam;
    memset(&dParam, 0, sizeof(DrawParam));

    dParam.render = &render_window;

    ToolsAddStrings(dParam.diffuse, 256, path, "/textures/super_hero.png");
    SpriteObjectInitDefault(&sprite, &dParam);
    Transform2DSetPosition(&sprite, 800, 800);
    Transform2DSetScale(&sprite, 50, 50);

    SpriteObjectSetOffsetRect(&sprite, o_x, o_y, 48.0, 48.0);
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

    LineParams line;
    line.position = (vec2){ 0, 0};
    line.direction = (vec2){ 1, 1};
    line.length = 500;
    line.color = (vec3){ 1, 1, 1};

    ShapeObjectInitDefault(&obj1, &dParam, ENGINE_SHAPE_OBJECT_SHAPE, &shape);
    Transform2DSetPosition(&obj1, 0, 0);
    ShapeObjectInitDefault(&obj2, &dParam, ENGINE_SHAPE_OBJECT_QUAD, &quad);
    Transform2DSetPosition(&obj2, 300, 300);
    ShapeObjectInitDefault(&obj3, &dParam, ENGINE_SHAPE_OBJECT_SHAPE, &shape);
    Transform2DSetPosition(&obj3, 550, 550);

    char font[256];
    ToolsAddStrings(font, 256, path, "/fonts/RobotoBlack.ttf");
    TextObjectInitDefault(&interText, 9, font, &dParam);
    Transform2DSetPosition(&interText, 200, 200);
    TextObjectSetText(&interText, "У попа была собака!");
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

        SpriteObjectSetOffsetRect(&sprite, o_x, o_y, 48.0, 48.0);

        time = 0;
    }


}

void SpriteUpdate(float deltaTime)
{
    AnimSprite(deltaTime);

    double mX, mY;

    EngineGetCursorPos(&mX, &mY);

    Transform2DSetPosition(&sprite, mX * 2, mY * 2);

    /*InterSquareParam param1;
    param1.position = Transform2DGetPosition(&obj1);
    param1.size = 100;*/

    InterRay2DParam ray;
    ray.direction = (vec2){ 1, 1};
    ray.distance = 500;
    ray.position = Transform2DGetPosition(&obj1);

    InterCircleParam circle;
    circle.center = (vec2){ 300, 300};
    circle.radius = 100;

    InterSquareParam box;
    box.position = (vec2){ 300, 300};
    box.size = 100;

    float t, t2;
    vec2 q;

    GJKObject gjk;

    int result = IntersectGJK2D(&gjk, &obj1, &obj3);

}

void SpriteDraw()
{
    EngineDraw(&sprite);

    //EngineDraw(&obj1);
    //EngineDraw(&obj2);
    //EngineDraw(&obj3);

    EngineDraw(&interText);
}


void SpriteDestroy()
{
    GameObjectDestroy(&sprite);

    GameObjectDestroy(&obj1);
    GameObjectDestroy(&obj2);
    GameObjectDestroy(&obj3);

    GameObjectDestroy(&interText);
}
