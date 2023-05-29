#include "painter.h"

#include "math.h"

#include <gameObject.h>

#include "textObject.h"
#include <e_painter.h>

#include <e_math.h>

#include "resource.h"

EPainter painter;

float some_x = 0, some_y = 0;

bool some_leftMouse = 0, some_rightMouse = 0, linked_mouse = 0;

vec2 vector_force, counter_force = { 0, 0};

float rub_speed = 0.5, back_speed = 0.1f;

void PainterMouseKeyCallback(void* window, int button, int action, int mods){
    if(button == ENGINE_MOUSE_BUTTON_LEFT && action == ENGINE_PRESS)
    {

        double xpos, ypos;

        EngineGetCursorPos(&xpos, &ypos);


        if(fabs(some_x - xpos) < 5 && abs(some_y - ypos) < 5)
            linked_mouse = true;

    }

    if(button == ENGINE_MOUSE_BUTTON_LEFT && action == ENGINE_RELEASE)
    {

        counter_force.x = -(250 - some_x) * rub_speed;
        counter_force.y = -(250 - some_y) * rub_speed;

        linked_mouse = false;
    }
}

void SomePainterFunc(EPainter *painter)
{
    PainterObjectSetColor(0.4, 0.2, 0.2);
    for(int i=0;i < 10;i++)
        PainterObjectMakeRectangle(painter, 0, i * 50, 20, 20);
    PainterObjectSetColor(0.2, 0.4, 0.2);
    PainterObjectMakeSegment(painter, 250, 250, some_x, some_y, 5);
    PainterObjectMakeCircle(painter, 350, 350, 10);
}

void PainterInit(){

    DrawParam dParam;
    memset(&dParam, 0, sizeof(DrawParam));
    dParam.render = &render_window;

    PainterObjectInitDefault(&painter, &dParam);

    some_x = some_y = 200;

    PainterObjectSetPaintFunc((void*)SomePainterFunc);
}

void PainterUpdate(){

    if(linked_mouse){
        double xpos, ypos;

        EngineGetCursorPos(&xpos, &ypos);
        some_x = xpos;
        some_y = ypos;
    }else{
        some_x -= back_speed;
        some_y -= back_speed;

        vector_force.x -=0.1f;
        vector_force.y -=0.1f;

        if(vector_force.x < -counter_force.x)
            counter_force.x = -(250 - some_x);

        if(vector_force.y < -counter_force.y)
            counter_force.y = -(250 - some_y);

    }
}

void PainterDraw(){

    EngineDraw(&painter);

}

void PainterDestroy(){
    GameObjectDestroy(&painter);

}
