#include "game.h"

#include "e_camera.h"

#include "game_sys.h"
#include "gui_test.h"
#include "meshes.h"
#include "sprites.h"
#include "painter.h"

#include "e_math.h"

#include "resource.h"

#include "e_resource_data.h"

float cascadeSplitLambda = 0.75;

void MouseKeyCallback(void* window, int button, int action, int mods)
{
    SysMouseKeyCallback(window, button, action, mods);
    PainterMouseKeyCallback(window, button, action, mods);
}

void Init(){

    Camera2DInit(&camera2D);
    Camera3DInit(&camera3D);

    RenderTextureInit(&render_window, ENGINE_RENDER_TYPE_WINDOW, 0, 0, 0);
    render_window.persp_view_distance = 2000;

    for (uint32_t i = 0; i < MAX_SOME_SHADOWS; i++)
        RenderTextureInit(&render_shadow[i], ENGINE_RENDER_TYPE_DEPTH, 2048, 2048, 0);

    for(int i=0; i < MAX_CUBE_SHADOWS;i++)
    {
        RenderTextureInit(&render_cube_shadow[i], ENGINE_RENDER_TYPE_CUBEMAP, 1024, 1024, ENGINE_RENDER_FLAG_DEPTH);
        render_cube_shadow[i].persp_view_near = 1.0f;
        render_cube_shadow[i].persp_view_distance = 100;
        render_cube_shadow[i].persp_view_angle = 90;
        render_cube_shadow[i].flags |= ENGINE_RENDER_FLAG_PERSPECTIVE;
    }

    for(int i=0; i < MAX_SPOT_SHADOWS;i++)
    {
        RenderTextureInit(&render_spot_shadow[i], ENGINE_RENDER_TYPE_DEPTH, 2048, 2048, 0);
        render_spot_shadow[i].persp_view_distance = 100;
        render_spot_shadow[i].persp_view_near = 1.0f;
        render_spot_shadow[i].persp_view_angle = 45;
        render_spot_shadow[i].flags |= ENGINE_RENDER_FLAG_PERSPECTIVE;
    }

    RenderTextureInit(&render_image, ENGINE_RENDER_TYPE_IMAGE, 2048, 2048, 0);

    EngineSetMouseKeyCallback((void *)MouseKeyCallback);

    SysInit();
    SysCalcDirLight();

    InitMeshes();
    //SpriteInit();
    //GUIInit();
    //PainterInit();
}

void UpdateCascades(float deltaTime)
{

    float cascadeSplits[MAX_SOME_SHADOWS];

    float nearClip = 1.0f;
    float farClip = 500;
    float clipRange = farClip - nearClip;

    float minZ = nearClip;
    float maxZ = nearClip + clipRange;

    float range = maxZ - minZ;
    float ratio = maxZ / minZ;

    // Calculate split depths based on view camera frustum
    // Based on method presented in https://developer.nvidia.com/gpugems/GPUGems3/gpugems3_ch10.html
    for (uint32_t i = 0; i < MAX_SOME_SHADOWS; i++) {
        float p = (float)(i + 1) / MAX_SOME_SHADOWS;
        float log = minZ * pow(ratio, p);
        float uniform = minZ + range * p;
        float d = cascadeSplitLambda * (log - uniform) + uniform;
        cascadeSplits[i] = (d - nearClip) / clipRange;
    }

    // Calculate orthographic projection matrix for each cascade
    float lastSplitDist = 0.0;
    for (uint32_t i = 0; i < MAX_SOME_SHADOWS; i++) {
        float splitDist = cascadeSplits[i];

        render_shadow[i].ortg_view_size = 0.025f * splitDist ;
        render_shadow[MAX_SOME_SHADOWS - i - 1].ortg_view_distance = farClip * splitDist;
        render_shadow[i].cascadeSplit = (nearClip + splitDist * clipRange) / 2.0f;

        RenderTexture *shadow = &render_shadow[i];

        lastSplitDist = cascadeSplits[i];
    }
}


void Update(double deltaTime){

    SysUpdate(deltaTime);
    UpdateMeshes(deltaTime);
    //SpriteUpdate(deltaTime);
    //GUIUpdate();
    //PainterUpdate();

    UpdateCascades(deltaTime);
}

void Draw(){

    DrawMeshes();
    //SpriteDraw();
    //GUIDraw();
    //PainterDraw();

    EngineSetRender( &render_image, 1);
    //EngineSetRender( render_shadow, MAX_SOME_SHADOWS);
    /*EngineSetRender( render_cube_shadow, MAX_CUBE_SHADOWS);
    EngineSetRender( render_spot_shadow, MAX_SPOT_SHADOWS);*/
    EngineSetRender( &render_window, 1);

    EngineLoop();
}



void CleanUp(){
    DestroyMeshes();
    //SpriteDestroy();
    //GUIDestroy();
    //PainterDestroy();

    RenderTextureDestroy(&render_window);

    for (uint32_t i = 0; i < MAX_SOME_SHADOWS; i++)
        RenderTextureDestroy(&render_shadow[i]);

    for (uint32_t i = 0; i < MAX_CUBE_SHADOWS; i++)
        RenderTextureDestroy(&render_cube_shadow[i]);

    for (uint32_t i = 0; i < MAX_SPOT_SHADOWS; i++)
        RenderTextureDestroy(&render_spot_shadow[i]);

    RenderTextureDestroy(&render_image);
}
