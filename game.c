#include "game.h"

#include "transform.h"

#include "camera.h"
#include <math.h>

SpriteObject* go;
SpriteObject* go2;
SpriteObject* go3;

GameObject3D* go3D;
GameObject3D* go3D2;

Camera2D camera2D;
Camera3D camera3D;

TextObject* to;
TextObject* to2;
TextObject* to3;

float yaw = -90.f, pitch = 0;

float cameraSpeed = 4.5f, sensitivity = 2.0f;

void key_update(float deltaTime)
{
    vec3 up = {0.0f,1.0f,0.0f};

    if (EngineGetKeyPress(GLFW_KEY_W)){
        vec3 pos = getViewPos();
        setViewPos(v3_sub(pos, v3_muls(getViewRotation(), cameraSpeed * deltaTime)));
    }else if (EngineGetKeyPress(GLFW_KEY_S)){
        vec3 pos = getViewPos();
        setViewPos(v3_add(pos, v3_muls(getViewRotation(), cameraSpeed * deltaTime)));
    }

    if (EngineGetKeyPress(GLFW_KEY_A)){
        vec3 pos = getViewPos();
        setViewPos(v3_sub(pos, v3_muls(v3_norm(v3_cross(getViewRotation(), up)), cameraSpeed * deltaTime)));
    }else if (EngineGetKeyPress(GLFW_KEY_D)){
        vec3 pos = getViewPos();
        setViewPos(v3_add(pos, v3_muls(v3_norm(v3_cross(getViewRotation(), up)), cameraSpeed * deltaTime)));
    }

    if (EngineGetKeyPress(GLFW_KEY_ESCAPE))
        glfwSetWindowShouldClose(window, GL_TRUE);
}

void someRotate(float deltaTime){

    double xpos, ypos;

    EngineGetcursorPos(&xpos, &ypos);
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

void Init(){

    EngineHideCursor(1);

    Camera2DInit(&camera2D);
    Camera3DInit(&camera3D);

    go = (SpriteObject *) calloc(1, sizeof(SpriteObject));
    initSpriteObject(go, (vec2){100.f, 100.f}, (vec2){0, 0.f}, "/home/ilia/Projects/Game/textures/texture.png", "/home/ilia/Projects/Game/shaders/Sprite/vert.spv", "/home/ilia/Projects/Game/shaders/Sprite/frag.spv");

    go2 = (SpriteObject *) calloc(1, sizeof(SpriteObject));
    initSpriteObject(go2, (vec2){200.f, 200.f}, (vec2){-100, 0.f}, "/home/ilia/Projects/Game/textures/texture.png", "/home/ilia/Projects/Game/shaders/Sprite/vert.spv", "/home/ilia/Projects/Game/shaders/Sprite/frag.spv");

    go3 = (SpriteObject *) calloc(1, sizeof(SpriteObject));
    initSpriteObject(go3, (vec2){600.f, 600.f}, (vec2){-100, 0.f}, "/home/ilia/Projects/Game/textures/texture.png", "/home/ilia/Projects/Game/shaders/Sprite/vert.spv", "/home/ilia/Projects/Game/shaders/Sprite/frag.spv");

    go3D = (PrimitiveObject *) calloc(1, sizeof(PrimitiveObject));
    PrimitiveObjectInit(go3D);

    go3D->transform.position.z = 6;
    go3D->transform.scale.z = 2;

    go3D2 = (PrimitiveObject *) calloc(1, sizeof(PrimitiveObject));
    PrimitiveObjectInit(go3D2);

    go3D2->transform.position.z = 8;
    go3D2->transform.scale.y = 2;
    go3D2->transform.scale.x = 2;

    to = (TextObject *) calloc(1, sizeof(TextObject));
    initTextObject(to, "/home/ilia/Projects/Game/fonts/null-normal.otf", "/home/ilia/Projects/Game/shaders/text/vert.spv", "/home/ilia/Projects/Game/shaders/text/frag.spv");
    SetTextColor(to, (vec3){1.0f,0.0f,0.0f});
    addTextW(L"Съешь еще этих французких булок, да выпей чаю!", to);
    setPosTransform2D(to, (vec2){50.0f, 50.f});

    to2 = (TextObject *) calloc(1, sizeof(TextObject));
    initTextObject(to2, "/home/ilia/Projects/Game/fonts/fantazer-normal.ttf", "/home/ilia/Projects/Game/shaders/text/vert.spv", "/home/ilia/Projects/Game/shaders/text/frag.spv");
    SetTextColor(to2, (vec3){0.0f,1.0f,0.0f});
    addText("Welcome to city 17!", to2);
    setPosTransform2D(to2, (vec2){-50.0f, -50.f});

    to3 = (TextObject *) calloc(1, sizeof(TextObject));
    initTextObject(to3, "/home/ilia/Projects/Game/fonts/fantazer-normal.ttf", "/home/ilia/Projects/Game/shaders/text/vert.spv", "/home/ilia/Projects/Game/shaders/text/frag.spv");
    SetTextColor(to3, (vec3){1.0f,0.0f,0.0f});
    addTextW(L"СЪЕШЬ ЕЩЕ ЭТИХ ФРАНЦУЗСКИХ БУЛОК, ДА ВЫПЕЙ ЧАЮ!", to3);
    setPosTransform2D(to3, (vec2){50.0f, 150.f});

    setViewPos((vec3){0.0f,0.0f, -5.0f});

}

void Update(float deltaTime){

    someRotate(deltaTime);

    float speed = 100;

    key_update(deltaTime);

    vec2 newPos;

    newPos = getPosTransform2D(go);
    newPos.x += 0.3f * deltaTime * speed;
    setPosTransform2D(go, newPos);
    newPos = getOffsetTransform(go);
    newPos.x += 0.5f * deltaTime * speed;
    newPos.y += 0.5f * deltaTime * speed;
    setOffsetTransform(go, newPos);

    newPos = getPosTransform2D(go2);
    newPos.x += 0.2f * deltaTime * speed;
    setPosTransform2D(go2, newPos);
    newPos = getOffsetTransform(go2);
    newPos.x += 10.f * deltaTime * speed;
    newPos.y += 10.f * deltaTime * speed;
    setOffsetTransform(go2, newPos);

    newPos = getPosTransform2D(go3);
    newPos.x += 0.1f * deltaTime * speed;
    setPosTransform2D(go3, newPos);
    newPos = getOffsetTransform(go3);
    newPos.x += 1.f * deltaTime * speed;
    newPos.y += 1.f * deltaTime * speed;
    setOffsetTransform(go3, newPos);

    go3D->transform.rotation.y += 1.f * deltaTime * speed;
    go3D->transform.rotation.x += 3.f * deltaTime * speed;

    if(getPosTransform2D(go).x > getViewPos2D().x + getWindowSize().x + getSizeGameObject(&go->go).x)
    {
        newPos.x = getViewPos2D().x - getWindowSize().x - getSizeGameObject(&go->go).x;
        newPos.y = 0;
        setPosTransform2D(go, newPos);
    }

    if(getPosTransform2D(go2).x  > getViewPos2D().x + getWindowSize().x + getSizeGameObject(&go2->go).x)
    {
        newPos.x = getViewPos2D().x - getWindowSize().x - getSizeGameObject(&go2->go).x;
        newPos.y = 0;
        setPosTransform2D(go2, newPos);
    }

    if(getPosTransform2D(go3).x > getViewPos2D().x + getWindowSize().x + getSizeGameObject(&go3->go).x)
    {
        newPos.x = getViewPos2D().x - getWindowSize().x - getSizeGameObject(&go3->go).x;
        newPos.y = 0;
        setPosTransform2D(go3, newPos);
    }

}

void Draw(){
    engDraw3D(go3D);
    engDraw3D(go3D2);

    engDraw(go);
    engDraw(go2);
    engDraw(go3);


    engDrawText(to);
    engDrawText(to2);
    engDrawText(to3);
}



void CleanUp(){
    destroyGameObject(go);
    destroyGameObject(go2);
    destroyGameObject(go3);

    GameObject3DDestroy(go3D);
    GameObject3DDestroy(go3D2);

    destroyTextObject(to);
    destroyTextObject(to2);
    destroyTextObject(to3);

    cleanUp();
}
