#include "meshes.h"

#include "resource.h"

#include "textObject.h"
#include "gameObject2D.h"
#include "gameObject3D.h"
#include "primitiveObject.h"
#include "projectionPlaneObject.h"
#include "models.h"

#include "buffers.h"
#include "texture.h"
#include "transform.h"

#include "e_math.h"

#include "physics.h"

TextObject* interText;

ProjectionPlaneObject projObj;

PrimitiveObject plane;

ModelObject3D teMO;
ModelObject3D teMO2;

void InitMeshes(){

    DrawParam dParam = {};
    memset(&dParam, 0, sizeof(dParam));

    char objpath[256];

    ToolsAddStrings(objpath, 256, path, "/models/rabochiy_i_kolkhoznitsa_lou_poli2.obj");
    ToolsAddStrings(dParam.filePath, 256, path, "/textures/rabochii_i_kolhoznitca/metal_defaultMat_Diffuse.png");
    dParam.drawType = 0;
    dParam.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    Load3DObjModel(&teMO, objpath, dParam);
    //AddPhyObject(&teMO.go, PHYSICS_COLLIDER_TYPE_MESH, PHYSICS_PHYSIC_STATIC, false);


    ToolsAddStrings(objpath, 256, path, "/models/TheRocketAnimation.fbx");
    ToolsAddStrings(dParam.filePath, 256, path, "/textures/hover_board/Color.png");
    Load3DFBXModel(&teMO2, objpath, dParam);
    Transform3DSetRotate(&teMO2, (vec3){ -90, 0, 0 });
    Transform3DSetScale(&teMO2, (vec3){ 1, 1, 1 });
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
    PlaneParam pParam;
    ToolsAddStrings(dParam.filePath, 256, path, "/textures/texture.png");
    memset(dParam.vertShader, 0, 256);
    memset(dParam.fragShader, 0, 256);
    dParam.drawType = 0;
    dParam.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    pParam.sectorCount = 200;
    pParam.stackCount = 200;
    PrimitiveObjectInit(&plane, scale, pos, dParam, ENGINE_PRIMITIVE3D_PLANE, &pParam);
    //AddPhyObject(&plane.go, PHYSICS_COLLIDER_TYPE_MESH, PHYSICS_PHYSIC_STATIC, false);

    float maxVal = 30, res = 0;

    vec3 movePlane = {0};

    for(int i=0;i < plane.go.graphObj.shape.vParam.verticesSize;i++){
        res = noise2D((plane.go.graphObj.shape.vParam.vertices[i].position.x + movePlane.x)/ maxVal, (plane.go.graphObj.shape.vParam.vertices[i].position.z + movePlane.z) / maxVal);
        plane.go.graphObj.shape.vParam.vertices[i].position.y = res * 10;

    }

}

void UpdateMeshes(float deltaTime){

}

void DrawMeshes(){

    engDraw(&projObj);

    //engDraw(&teMO);
    engDraw(&teMO2);

    engDraw(&plane);

}

void DestroyMeshes(){

    DestroyOBJModel(&teMO);
    DestroyFBXModel(&teMO2);

    GameObject2DDestroy(&projObj);

    GameObject3DDestroy(&plane);

}
