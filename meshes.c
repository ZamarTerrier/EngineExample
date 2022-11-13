#include "meshes.h"

#include <time.h>

#include "engine.h"

#include "resource.h"

#include "textObject.h"
#include "gameObject2D.h"
#include "gameObject3D.h"
#include "primitiveObject.h"
#include "projectionPlaneObject.h"
#include "lightObject.h"
#include "particleSystem.h"

#include "objLoader.h"
#include "fbxLoader.h"
#include "glTFLoader.h"

#include "buffers.h"
#include "texture.h"
#include "transform.h"

#include "e_math.h"

#include "physics.h"

#include "e_audioObject.h"

ProjectionPlaneObject projObj;

ParticleObject3D particle;

PrimitiveObject plane;
PrimitiveObject l_object;
PrimitiveObject skybox;

ModelObject3D teMO;

LightObject light;

AudioObject audio;

TextObject interText;

float timer;

void InitMeshes(){

    DrawParam dParam = {};
    memset(&dParam, 0, sizeof(dParam));

    char objpath[256];

    ToolsAddStrings(dParam.diffuse, 256, path, "/textures/texture.jpg");
    ToolsAddStrings(dParam.specular, 256, path, "/textures/specular.png");
    ToolsAddStrings(dParam.normal, 256, path, "/textures/normal_map.png");

    ToolsAddStrings(objpath, 256, path, "/models/");
    Load3DglTFModel(&teMO, objpath, "Hover_Animated", 2, dParam);
    Transform3DSetRotateT(&teMO.transform, 0, 0, 0);
    Transform3DSetScaleT(&teMO.transform, 0.1, 0.1, 0.1);
    Transform3DSetPositionT(&teMO.transform, 1, 0, 1);
    //AddPhyObject(&teMO.go, PHYSICS_COLLIDER_TYPE_MESH, PHYSICS_PHYSIC_STATIC, false);

    PlaneParam pParam;
    pParam.sectorCount = 20;
    pParam.stackCount = 20;
    PrimitiveObjectInit(&plane, dParam, ENGINE_PRIMITIVE3D_PLANE, &pParam);
    Transform3DSetPosition(&plane, 0, 0, 0);
    GameObject3DEnableLight(&plane, true);

    SphereParam sParam;
    sParam.radius = 0.1f;
    sParam.sectorCount = 10;
    sParam.stackCount = 10;
    PrimitiveObjectInit(&l_object, dParam, ENGINE_PRIMITIVE3D_SPHERE, &sParam);

    sParam.radius = 1.0f;
    sParam.sectorCount = 20;
    sParam.stackCount = 20;
    ToolsAddStrings(dParam.diffuse, 256, path, "/textures/sky.jpg");
    memset(dParam.specular, 0, 256);
    memset(dParam.normal, 0, 256);
    PrimitiveObjectInit(&skybox, dParam, ENGINE_PRIMITIVE3D_SKYBOX, &sParam);

    LightObjectInit(&light, ENGINE_LIGHT_TYPE_POINT);
    LightObjectSetPosition(&light, 1, 1, 1);
    LightObjectSetAmbientColor(&light, 0.4f, 0.4f, 0.4f);
    LightObjectSetDiffuseColor(&light, 0.2f, 0.2f, 0.2f);
    LightObjectSetSpecularColor(&light, 0.4f, 0.4f, 0.4f);

    ToolsAddStrings(dParam.diffuse, 256, path, "/textures/bubble2.png");
    Particle3DInit(&particle, dParam);

    AudioObjectInit(&audio);

    char buff[256];
    ToolsAddStrings(buff, 256, path, "/sounds/jump.wav");
    AudioObjectLoadFile(&audio, buff);
    audio.volume = 0.1f;

    char font[256];
    ToolsAddStrings(font, 256, path, "/fonts/arial.ttf");
    TextObjectInit(&interText, 9, font);

}

void UpdateMeshes(float deltaTime){

    double g_time = EngineGetTime();

    srand((uint32_t)(g_time * 1000));

    Load3DglTFNextFrame(&teMO, deltaTime, 0);


    float x = (cos(g_time / 10) + 1) * 4;
    float y = 1;
    float z = (sin(g_time / 10) + 1) * 4;

    LightObjectSetPosition(&light, x, y, z);

    Transform3DSetPosition(&l_object, x, y, z);

    if(timer > 5)
    {
        for(int i=0; i < 8;i++)
        {
            x = (float)rand() / (RAND_MAX / 2.0f);
            y = 5.0f + (float)rand() / (RAND_MAX / 10.0f);
            z = (float)rand() / (RAND_MAX / 2.0f);
            vec3 dir = { x, y, z};
            y = 0;
            vec3 position = { x, y, z};

            Particle3DAdd(&particle, position, dir, 2 * deltaTime, 2 * deltaTime, 5);
        }

        timer = 0;
    }else
        timer += 0.1f;


    AudioObjectSetSourcePosition3D(&audio, Transform3DGetPosition(&l_object));
    AudioObjectPlaySound(&audio, 0);

}

void DrawMeshes(){

    LightObjectDraw(&light);

    engDraw(&teMO);

    engDraw(&plane);
    engDraw(&l_object);
    engDraw(&skybox);

    engDraw(&particle);

    engDraw(&interText);
}

void DestroyMeshes(){

    GameObjectDestroy(&teMO);
    GameObjectDestroy(&plane);
    GameObjectDestroy(&l_object);
    GameObjectDestroy(&skybox);
    GameObjectDestroy(&particle);

    AudioObjectDestroy(&audio);

    GameObjectDestroy(&interText);
}
