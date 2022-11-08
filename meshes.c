#include "meshes.h"

#include "resource.h"

#include "textObject.h"
#include "gameObject2D.h"
#include "gameObject3D.h"
#include "primitiveObject.h"
#include "projectionPlaneObject.h"
#include "objLoader.h"
#include "fbxLoader.h"
#include "glTFLoader.h"

#include "buffers.h"
#include "texture.h"
#include "transform.h"

#include "e_math.h"

#include "physics.h"

TextObject* interText;

ProjectionPlaneObject projObj;

PrimitiveObject plane;

ModelObject3D teMO;

void InitMeshes(){

    DrawParam dParam = {};
    memset(&dParam, 0, sizeof(dParam));

    char objpath[256];

    ToolsAddStrings(objpath, 256, path, "/models/");
    ToolsAddStrings(dParam.filePath, 256, path, "/textures/texture.jpg");
    Load3DglTFModel(&teMO, objpath, "Hover_Animated", 2, dParam);
    Transform3DSetRotateT(&teMO.transform, 0, 0, 0);
    Transform3DSetScaleT(&teMO.transform, 0.1, 0.1, 0.1);
    //AddPhyObject(&teMO.go, PHYSICS_COLLIDER_TYPE_MESH, PHYSICS_PHYSIC_STATIC, false);

    PlaneParam pParam;
    pParam.sectorCount = 20;
    pParam.stackCount = 20;
    PrimitiveObjectInit(&plane, dParam, ENGINE_PRIMITIVE3D_PLANE, &pParam);

}

void UpdateMeshes(float deltaTime){
    Load3DglTFNextFrame(&teMO, deltaTime, 0);
}

void DrawMeshes(){

    engDraw(&teMO);

    engDraw(&plane);

}

void DestroyMeshes(){

    GameObjectDestroy(&teMO);

    GameObjectDestroy(&plane);

}
