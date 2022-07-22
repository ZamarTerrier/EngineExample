#include "game.h"

#include "primitiveObject.h"
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

float speed = 100;

bool lock_cursor = true, esc_press = false;

char path[] = "/home/ilia/Projects/Game";

void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods){
    if ( key == GLFW_KEY_ESCAPE && action == GLFW_PRESS && !esc_press){

        if(lock_cursor)
            EngineHideCursor(2);
        else
            EngineHideCursor(0);

        lock_cursor = !lock_cursor;

        esc_press = true;

    }else if(key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
        esc_press = false;
}

void KeyUpdateInput(float deltaTime)
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


}

void CamRotateView(float deltaTime){

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

void InitText(){

    char *font;
    char *vertShdr;
    char *fragShdr;

    font = add_string(path, "/fonts/null-normal.otf");
    vertShdr = add_string(path,"/shaders/text/vert.spv");
    fragShdr = add_string(path,"/shaders/text/frag.spv");

    to = (TextObject *) calloc(1, sizeof(TextObject));
    initTextObject(to, font, vertShdr, fragShdr);
    SetTextColor(to, (vec3){1.0f,0.0f,0.0f});
    addTextW(L"Съешь еще этих французких булок, да выпей чаю!", to);
    setPosTransform2D(to, (vec2){50.0f, 50.f});

    free(font);
    font = add_string(path, "/fonts/fantazer-normal.ttf");

    to2 = (TextObject *) calloc(1, sizeof(TextObject));
    initTextObject(to2, font, vertShdr, fragShdr);
    SetTextColor(to2, (vec3){0.0f,1.0f,0.0f});
    addText("Welcome to city 17!", to2);
    setPosTransform2D(to2, (vec2){-50.0f, -50.f});

    to3 = (TextObject *) calloc(1, sizeof(TextObject));
    initTextObject(to3, font, vertShdr, fragShdr);
    SetTextColor(to3, (vec3){1.0f,0.0f,0.0f});
    addTextW(L"СЪЕШЬ ЕЩЕ ЭТИХ ФРАНЦУЗСКИХ БУЛОК, ДА ВЫПЕЙ ЧАЮ!", to3);
    setPosTransform2D(to3, (vec2){50.0f, 150.f});

    setViewPos((vec3){0.0f,0.0f, -5.0f});

}

void Init2DObjects(){

    char *texture;
    char *vertShdr;
    char *fragShdr;

    texture = add_string(path,"/textures/texture.png");
    vertShdr = add_string(path,"/shaders/Sprite/vert.spv");
    fragShdr = add_string(path,"/shaders/Sprite/frag.spv");

    go = (SpriteObject *) calloc(1, sizeof(SpriteObject));
    initSpriteObject(go, (vec2){100.f, 100.f}, (vec2){0, 0.f}, texture, vertShdr, fragShdr);

    go2 = (SpriteObject *) calloc(1, sizeof(SpriteObject));
    initSpriteObject(go2, (vec2){200.f, 200.f}, (vec2){-100, 0.f}, texture, vertShdr, fragShdr);

    go3 = (SpriteObject *) calloc(1, sizeof(SpriteObject));
    initSpriteObject(go3, (vec2){600.f, 600.f}, (vec2){-100, 0.f}, texture, vertShdr, fragShdr);

}

void Init3DObjects(){

    char *texture;
    char *vertShdr;
    char *fragShdr;

    vertShdr = add_string(path,"/shaders/3DObject/vert.spv");
    fragShdr = add_string(path,"/shaders/3DObject/frag.spv");
    texture = add_string(path, "/textures/texture2.png");

    go3D = (PrimitiveObject *) calloc(1, sizeof(PrimitiveObject));
    PrimitiveObjectInit(go3D, (vec3){1.f, 1.f, 2.f}, (vec3){0, 0.f, 6.f}, texture, vertShdr, fragShdr, ENGINE_PRIMITIVE3D_CUBE);


    go3D2 = (PrimitiveObject *) calloc(1, sizeof(PrimitiveObject));
    PrimitiveObjectInit(go3D2, (vec3){2.f, 2.f, 1.f}, (vec3){0, 0.f, 8.f}, texture, vertShdr, fragShdr, ENGINE_PRIMITIVE3D_QUAD);

}

void Init(){

    EngineSetKeyCallback((void *)KeyCallback);

    EngineHideCursor(1);

    Camera2DInit(&camera2D);
    Camera3DInit(&camera3D);

    Init2DObjects();
    Init3DObjects();
    InitText();
}

void Update2D(float deltaTime){
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

void Update3D(float deltaTime){

    vec3 rot = getRotateTransform3D(go3D);
    rot.y += 1.f * deltaTime * speed;
    rot.x += 3.f * deltaTime * speed;
    setRotateTransform3D(go3D, rot);

}

void Update(float deltaTime){


    if(lock_cursor){
        CamRotateView(deltaTime);

        KeyUpdateInput(deltaTime);
    }

    Update2D(deltaTime);

    Update3D(deltaTime);

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
