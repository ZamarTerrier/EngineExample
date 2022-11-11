#include "game.h"

#include "camera.h"

#include "game_sys.h"
#include "gui_test.h"
#include "meshes.h"
#include "sprites.h"

#include "resource.h"

void Init(){

    Camera2DInit(&camera2D);
    Camera3DInit(&camera3D);

    SysInit();

    InitMeshes();
    GUIInit();
    //SpriteInit();
}


void Update(float deltaTime){

    SysUpdate(deltaTime);
    UpdateMeshes(deltaTime);
    //GUIUpdate();
    //SpriteUpdate(deltaTime);
}

void Draw(){
    DrawMeshes();
    //GUIDraw();
    //SpriteDraw();
}



void CleanUp(){
    DestroyMeshes();
    //GUIDestroy();
    //SpriteDestroy();
}
