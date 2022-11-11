#include "game_sys.h"

#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "e_math.h"
#include "transform.h"
#include "resource.h"

void MouseKeyCallback(GLFWwindow* window, int button, int action, int mods){


    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        leftMouse = true;
    else  if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
            leftMouse = false;
}

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){

    if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS && !esc_press){

        if(lock_cursor)
            EngineHideCursor(2);
        else
            EngineHideCursor(1);

        lock_cursor = !lock_cursor;

        esc_press = true;

    }else if(key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
        esc_press = false;
}

void KeyUpdateInput(float deltaTime)
{
    vec3 up = {0.0f,1.0f,0.0f};

    vec3 pos = getViewPos();
    vec3 viewRot = getViewRotation();

    if (EngineGetKeyPress(GLFW_KEY_W)){
        if(walk)
            viewRot.y = 0;
        vec3 forward = v3_sub(pos, v3_muls( viewRot, cameraSpeed * deltaTime));

        setViewPos(forward);
    }else if (EngineGetKeyPress(GLFW_KEY_S)){
        if(walk)
            viewRot.y = 0;
        vec3 forward = v3_add(pos, v3_muls( viewRot, cameraSpeed * deltaTime));
        setViewPos(forward);
    }


    if (EngineGetKeyPress(GLFW_KEY_A)){
        setViewPos(v3_sub(pos, v3_muls(v3_norm(v3_cross(viewRot, up)), cameraSpeed * deltaTime)));
    }else if (EngineGetKeyPress(GLFW_KEY_D)){
        setViewPos(v3_add(pos, v3_muls(v3_norm(v3_cross(viewRot, up)), cameraSpeed * deltaTime)));
    }

    if (EngineGetKeyPress(GLFW_KEY_LEFT_SHIFT)){
        cameraSpeed = moveSpeed * 10;
    }else{
        cameraSpeed = moveSpeed * 3;
    }

    if (EngineGetKeyPress(GLFW_KEY_1)){
        walk = true;
    }else if (EngineGetKeyPress(GLFW_KEY_2)){
        walk = false;
    }

    if( EngineGetKeyPress(GLFW_KEY_F1)){
        showconsole = true;
    }else if(EngineGetKeyPress(GLFW_KEY_F2)){
        showconsole = false;
    }


    if (EngineGetKeyPress(GLFW_KEY_SPACE) && force <= 0 && grounded){
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

    vec3 direction = getViewRotation();
    direction.x = cos(yaw * (M_PI / 180)) * cos(pitch * (M_PI / 180));
    direction.y = -sin(pitch * (M_PI / 180));
    direction.z = sin(yaw * (M_PI / 180)) * cos(pitch * (M_PI / 180));
    setViewRotation(direction);
}


void SysInit()
{

    EngineHideCursor(1);

    EngineSetKeyCallback((void *)KeyCallback);
    EngineSetMouseKeyCallback((void *)MouseKeyCallback);

}

void SysUpdate(float deltaTime)
{
    if(lock_cursor){
        CamRotateView(deltaTime);

        KeyUpdateInput(deltaTime);
    }
}