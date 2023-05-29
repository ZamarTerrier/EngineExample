#include "game_sys.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "engine.h"

#include "e_math.h"
#include "e_transform.h"
#include "resource.h"

#include "meshes.h"

#include "e_resource_data.h"

float some_yaw = 35.0;
float some_pitch = 65;

void SysMouseKeyCallback(void* window, int button, int action, int mods){

    if (button == ENGINE_MOUSE_BUTTON_LEFT && action == ENGINE_PRESS)
    {
        if(!leftMouse)
            //SetupBall();

        leftMouse = true;
    }
    else  if (button == ENGINE_MOUSE_BUTTON_LEFT && action == ENGINE_RELEASE){
            leftMouse = false;
    }
}

void SysCalcDirLight()
{
    if(some_pitch > 89.0f)
        some_pitch = 89.0f;
    if(some_pitch < -89.0f)
        some_pitch = -89.0f;

    light_direction.x = cos(some_yaw * (M_PI / 180)) * cos(some_pitch * (M_PI / 180));
    light_direction.y = -sin(some_pitch * (M_PI / 180));
    light_direction.z = sin(some_yaw * (M_PI / 180)) * cos(some_pitch * (M_PI / 180));
}

void KeyCallback(void* window, int key, int scancode, int action, int mods){

    if ( (key == ENGINE_KEY_ESCAPE || key == -1)  && action == ENGINE_PRESS && !esc_press){

        if(lock_cursor)
            EngineHideCursor(2);
        else
            EngineHideCursor(1);

        lock_cursor = !lock_cursor;

        esc_press = true;

    }else if(key == ENGINE_KEY_ESCAPE && action == ENGINE_RELEASE)
        esc_press = false;

    if(key == ENGINE_KEY_LEFT)
    {
        some_yaw -=1;
    }else if(key == ENGINE_KEY_RIGHT)
    {
        some_yaw +=1;
    }else if(key == ENGINE_KEY_UP)
    {
        some_pitch +=1;
    }else if(key == ENGINE_KEY_DOWN)
    {
        some_pitch -=1;
    }

    SysCalcDirLight();

}

void KeyUpdateInput(double deltaTime)
{
    vec3 up = {0.0f,1.0f,0.0f};

    vec3 pos = Camera3DGetPosition();
    vec3 viewRot = Camera3DGetRotation();
    vec3 some_pos;

    if (EngineGetKeyPress(ENGINE_KEY_W)){
        if(walk)
            viewRot.y = 0;
        some_pos = v3_sub(pos, v3_muls( viewRot, cameraSpeed * deltaTime));

        Camera3DSetPosition(some_pos.x, some_pos.y, some_pos.z);
    }else if (EngineGetKeyPress(ENGINE_KEY_S)){
        if(walk)
            viewRot.y = 0;
        some_pos = v3_add(pos, v3_muls( viewRot, cameraSpeed * deltaTime));
        Camera3DSetPosition(some_pos.x, some_pos.y, some_pos.z);
    }


    if (EngineGetKeyPress(ENGINE_KEY_A)){
        some_pos = v3_sub(pos, v3_muls(v3_norm(v3_cross(viewRot, up)), cameraSpeed * deltaTime));
        Camera3DSetPosition(some_pos.x, some_pos.y, some_pos.z);
    }else if (EngineGetKeyPress(ENGINE_KEY_D)){
        some_pos = v3_add(pos, v3_muls(v3_norm(v3_cross(viewRot, up)), cameraSpeed * deltaTime));
        Camera3DSetPosition(some_pos.x, some_pos.y, some_pos.z);
    }

    if (EngineGetKeyPress(ENGINE_KEY_LEFT_SHIFT)){
        cameraSpeed = moveSpeed * 10;
    }else{
        cameraSpeed = moveSpeed * 3;
    }

    if (EngineGetKeyPress(ENGINE_KEY_1)){
        walk = true;
    }else if (EngineGetKeyPress(ENGINE_KEY_2)){
        walk = false;
    }

    if( EngineGetKeyPress(ENGINE_KEY_F1)){
        showconsole = true;
    }else if(EngineGetKeyPress(ENGINE_KEY_F2)){
        showconsole = false;
    }


    if (EngineGetKeyPress(ENGINE_KEY_SPACE) && force <= 0 && grounded){
        jump = true;
        force = 5;
    }
}

void CamRotateView(float deltaTime){

    double xpos, ypos;

    EngineGetCursorPos(&xpos, &ypos);
    EngineFixedCursorCenter();

    float xoffset = WIDTH/2 - xpos;
    float yoffset = HEIGHT/2 - ypos;

    xoffset *= sensitivity * deltaTime;
    yoffset *= sensitivity * deltaTime;

    yaw   += xoffset;
    pitch += yoffset;

    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    vec3 direction = Camera3DGetRotation();
    direction.x = cos(yaw * (M_PI / 180)) * cos(pitch * (M_PI / 180));
    direction.y = -sin(pitch * (M_PI / 180));
    direction.z = sin(yaw * (M_PI / 180)) * cos(pitch * (M_PI / 180));
    Camera3DSetRotation(direction.x, direction.y, direction.z);
}


void SysInit()
{

    if(lock_cursor)
        EngineHideCursor(1);

    EngineSetKeyCallback((void *)KeyCallback);

}

void SysUpdate(double deltaTime)
{
    if(lock_cursor){
        CamRotateView(deltaTime);

        KeyUpdateInput(deltaTime);
    }
}
