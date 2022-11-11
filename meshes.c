#include "meshes.h"

#include "engine.h"

#include "resource.h"

#include "textObject.h"
#include "gameObject2D.h"
#include "gameObject3D.h"
#include "primitiveObject.h"
#include "projectionPlaneObject.h"

#include "lightObject.h"

#include "objLoader.h"
#include "fbxLoader.h"
#include "glTFLoader.h"

#include "buffers.h"
#include "texture.h"
#include "transform.h"

#include "e_math.h"

#include "physics.h"

ProjectionPlaneObject projObj;

PrimitiveObject plane;
PrimitiveObject l_object;

ModelObject3D teMO;

LightObject light;
LightObject dir;

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
    //AddPhyObject(&teMO.go, PHYSICS_COLLIDER_TYPE_MESH, PHYSICS_PHYSIC_STATIC, false);

<<<<<<< HEAD
    ToolsAddStrings(objpath, 256, path, "/models/Hover_Rocket.gltf");
    ToolsAddStrings(binpath, 256, path, "/models/Hover_Rocket.bin");
    ToolsAddStrings(dParam.filePath, 256, path, "/textures/hover_board/Color.png");
//    ToolsAddStrings(objpath, 256, path, "/models/Walking_Doctor.gltf");
//    ToolsAddStrings(binpath, 256, path, "/models/Walking_Doctor.bin");
//    ToolsAddStrings(dParam.filePath, 256, path, "/textures/bode1.png");
    Load3DglTFModel(&teMO2, objpath, binpath, dParam);
    Transform3DSetRotateT(&teMO2.transform, (vec3){ 0, 0, 0 });
    Transform3DSetScaleT(&teMO2.transform, (vec3){ -1, 1, 1});
//    Transform3DSetRotateT(&teMO2.transform, (vec3){ 0, 0, 0 });
//    Transform3DSetScaleT(&teMO2.transform, (vec3){ 1, 1, 1});
    //AddPhyObject(&teMO.go, PHYSICS_COLLIDER_TYPE_MESH, PHYSICS_PHYSIC_STATIC, false);

    vec3 scale = {1, 1, 1};
    vec3 pos = {0, 0, 0};

    ToolsAddStrings(dParam.filePath, 256, path, "/textures/texture.png");
    ToolsAddStrings(dParam.vertShader, 256, path, "/shaders/ProjPlane/vert.spv");
    ToolsAddStrings(dParam.fragShader, 256, path, "/shaders/ProjPlane/frag.spv");
    dParam.drawType = 0;
    dParam.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    ProjectionPlaneInit(&projObj.go, dParam);

    scale = (vec3){1, 1, 1};
    pos = (vec3){0, 0, 0};
=======
>>>>>>> 4fe935e8e574a50cceb42ea9ade63af9ec091ba4
    PlaneParam pParam;
    pParam.sectorCount = 20;
    pParam.stackCount = 20;
    PrimitiveObjectInit(&plane, dParam, ENGINE_PRIMITIVE3D_PLANE, &pParam);
    Transform3DSetPosition(&plane, 0, 0, 0);

    SphereParam sParam;
    sParam.radius = 0.1f;
    sParam.sectorCount = 10;
    sParam.stackCount = 10;
    PrimitiveObjectInit(&l_object, dParam, ENGINE_PRIMITIVE3D_SPHERE, &sParam);
    l_object.go.enable_light = 0;

    LightObjectInit(&light, ENGINE_LIGHT_TYPE_POINT);
    LightObjectSetPosition(&light, 1, 1, 1);
    LightObjectSetAmbientColor(&light, 0.1f, 0.1f, 0.1f);
    LightObjectSetDiffuseColor(&light, 0.1f, 0.1f, 0.1f);
    LightObjectSetSpecularColor(&light, 0.1f, 0.1f, 0.1f);


    LightObjectInit(&dir, ENGINE_LIGHT_TYPE_DIRECTIONAL);
    LightObjectSetDirection(&dir, 0, -1.0f, 1.0f);
    LightObjectSetAmbientColor(&dir, 0.1f, 0.1f, 0.1f);
    LightObjectSetDiffuseColor(&dir, 0.1f, 0.1f, 0.1f);
    LightObjectSetSpecularColor(&dir, 0.1f, 0.1f, 0.1f);

    light.constant = 1.0f;

}

void UpdateMeshes(float deltaTime){
    Load3DglTFNextFrame(&teMO, deltaTime, 0);

    double time = EngineGetTime();

    float x = (cos(time) + 1) * 4;
    float y = 1;
    float z = (sin(time) + 1) * 4;

    LightObjectSetPosition(&light, x, y, z);

    Transform3DSetPosition(&l_object, x, y, z);
}

void DrawMeshes(){

    LightObjectDraw(&light);
//    LightObjectDraw(&dir);

    engDraw(&teMO);
<<<<<<< HEAD
    engDraw(&teMO2);
=======
>>>>>>> 4fe935e8e574a50cceb42ea9ade63af9ec091ba4

    engDraw(&plane);
    engDraw(&l_object);

}

void DestroyMeshes(){

    GameObjectDestroy(&teMO);
    GameObjectDestroy(&plane);
    GameObjectDestroy(&l_object);

}
