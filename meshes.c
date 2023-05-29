#include "meshes.h"

#include <stdlib.h>
#include <time.h>

#include "engine.h"

#include "resource.h"

#include "projectionPlaneObject.h"
#include "textObject.h"
#include "gameObject2D.h"
#include "gameObject3D.h"
#include "sky_object.h"
#include "primitiveObject.h"
#include "terrain_object.h"
#include "lightObject.h"
#include "quad_object.h"
#include "tree_object.h"
#include "marchingcube_object.h"

#include "models.h"
#include "glTFLoader.h"

#include "e_blue_print.h"

#include "e_buffer.h"
#include "e_texture.h"
#include "e_transform.h"

#include "e_math.h"

#include "physics.h"

#include "e_resource_data.h"
#include "e_resource_export.h"

#define TEXTURE_HEIGHT 2048
#define TEXTURE_WIDTH 2048

#define MAX_TREES 500
#define MAX_CUST 2000

TerrainObject terrain;

SkyObject skybox;

PrimitiveObject plane;
PrimitiveObject ball;
PrimitiveObject box;
PrimitiveObject box2;

PrimitiveObject f_obj;
PrimitiveObject b_obj;
PrimitiveObject l_obj;
PrimitiveObject r_obj;

MarchingCubeObject mco;

TextObject text;

ProjectionPlaneObject texture_plane;

QuadObject quad;

InterSphereParam player_col;

LightObject dir_light;
LightObject point_light;
LightObject point_light2;
LightObject spot_light;
LightObject spot_light2;

ModelObject3D mo;

TreeObject trees[MAX_TREES];
uint32_t num_trees = 0;

TreeObject cust[MAX_CUST];
uint32_t num_cust = 0;

uint32_t *some_map;

char **textures;
uint32_t num_textures = 0;

void AddTextureToTextures(TerrainParam * tParam, char * name)
{
    tParam->textures[tParam->t_t_param.num_textures] = ToolsMakeString(path, name);
    textures[num_textures] = ToolsMakeString(path, name);

    tParam->t_t_param.num_textures++;
    num_textures ++;
}

void InitMeshes(){

    EngineAcceptShadow(render_shadow, MAX_SOME_SHADOWS, ENGINE_SHADOW_TYPE_DIRECTIONAL);
    EngineAcceptShadow(render_cube_shadow, MAX_CUBE_SHADOWS, ENGINE_SHADOW_TYPE_POINT);
    EngineAcceptShadow(render_spot_shadow, MAX_SPOT_SHADOWS, ENGINE_SHADOW_TYPE_SPOT);

    //Параметры
    DrawParam dParam = {};
    memset(&dParam, 0, sizeof(dParam));

    ToolsAddStrings(dParam.diffuse, 256, path, "/textures/grass.jpg");
    ToolsAddStrings(dParam.normal, 256, path, "/textures/normal_map.png");
    //ToolsAddStrings(dParam.specular, 256, path, "/textures/specular.png");

    dParam.render = &render_window;

    dParam.flags = ENGINE_DRAW_PARAM_FLAG_DRAW_INDEXED; // | ENGINE_DRAW_PARAM_FLAG_ADD_SHADOW;

    //Небо
    SkyObjectInit(&skybox, &dParam, ENGINE_SKY_TYPE_ATMOSPHERIC);
    SkyObjectAddDefault(&skybox, &render_window);
    GameObject2DInitDraw(&skybox);

    char *model_path = ToolsMakeString(path, "/models/");

    Load3DglTFModel(&mo, model_path, "Little_Tokyo", 2, &dParam);
    Transform3DSetScaleT(&mo.transform, -0.1, 0.1, 0.1);
    ModelSetLightEnable(&mo, false);

    free(model_path);

    QuadObjectInit(&quad, ENGINE_QUAD_TYPE_IMAGE);
    QuadObjectAddDefault(&quad, &render_window);
    BluePrintAddRenderImage(&quad.go.graphObj.blueprints, 0, &render_image);
    GameObject2DInitDraw(&quad);
    Transform2DSetPosition(&quad, -300, -200);
    Transform2DSetScale(&quad, 400, 400);

    PlaneParam pParam;
    pParam.sectorCount = 50;
    pParam.stackCount = 50;
    PrimitiveObjectInitDefault(&plane, &dParam, ENGINE_PRIMITIVE3D_PLANE, &pParam);
    GameObject3DEnableLight(&plane, true);

    //Сферы для проверки столкновений
    ToolsAddStrings(dParam.diffuse, 256, path, "/textures/wave.png");

    SphereParam sParam;
    sParam.radius = 5.0f;
    sParam.sectorCount = 20;
    sParam.stackCount = 20;
    PrimitiveObjectInitDefault(&ball, &dParam, ENGINE_PRIMITIVE3D_SPHERE, &sParam);
    Transform3DSetPosition(&ball, 10, 30, 20);
    GameObject3DEnableLight(&ball, true);

    sParam.radius = 1.0f;
    PrimitiveObjectInitDefault(&box, &dParam, ENGINE_PRIMITIVE3D_SPHERE, &sParam);
    Transform3DSetPosition(&box, 5, 10, 10);
    GameObject3DEnableLight(&box, true);

    PrimitiveObjectInitDefault(&box2, &dParam, ENGINE_PRIMITIVE3D_CUBE, NULL);
    Transform3DSetScale(&box2, -40, -40, -40);
    GameObject3DEnableLight(&box2, true);

    PrimitiveObjectInitDefault(&f_obj, &dParam, ENGINE_PRIMITIVE3D_CUBE, NULL);
    Transform3DSetPosition(&f_obj, 0, 0, 10);
    GameObject3DEnableLight(&f_obj, true);

    PrimitiveObjectInitDefault(&b_obj, &dParam, ENGINE_PRIMITIVE3D_CUBE, NULL);
    Transform3DSetPosition(&b_obj, 0, 0, -10);
    GameObject3DEnableLight(&b_obj, true);

    PrimitiveObjectInitDefault(&l_obj, &dParam, ENGINE_PRIMITIVE3D_CUBE, NULL);
    Transform3DSetPosition(&l_obj, -10, 0, 0);
    GameObject3DEnableLight(&l_obj, true);

    PrimitiveObjectInitDefault(&r_obj, &dParam, ENGINE_PRIMITIVE3D_CUBE, NULL);
    Transform3DSetPosition(&r_obj, 10, 0, 0);
    GameObject3DEnableLight(&r_obj, true);

    LightObjectInit(&dir_light, ENGINE_LIGHT_TYPE_DIRECTIONAL);
    LightObjectSetAmbientColor(&dir_light, 0.01f, 0.01f, 0.01f);
    LightObjectSetDiffuseColor(&dir_light, 0.09f, 0.09f, 0.09f);
    LightObjectSetDirection(&dir_light, 0.3, 0.5, 0);

    LightObjectInit(&point_light, ENGINE_LIGHT_TYPE_POINT);
    LightObjectSetAmbientColor(&point_light, 0.01f, 0.01f, 0.01f);
    //LightObjectSetDiffuseColor(&point_light, 0.4f, 1.0f, 0.4f);
    LightObjectSetDiffuseColor(&point_light, 1.0f, 0.4f, 0.4f);
    LightObjectSetPosition(&point_light, 0, 4, 0);

    LightObjectInit(&point_light2, ENGINE_LIGHT_TYPE_POINT);
    LightObjectSetAmbientColor(&point_light2, 0.01f, 0.01f, 0.01f);
    LightObjectSetDiffuseColor(&point_light2, 0.4f, 0.1f, 0.4f);
    LightObjectSetPosition(&point_light2, 0, 2, 0);

    LightObjectInit(&spot_light, ENGINE_LIGHT_TYPE_SPOT);
    LightObjectSetAmbientColor(&spot_light, 0.01f, 0.01f, 0.01f);
    LightObjectSetDiffuseColor(&spot_light, 1.0f, 0.4f, 0.4f);

    LightObjectInit(&spot_light2, ENGINE_LIGHT_TYPE_SPOT);
    LightObjectSetAmbientColor(&spot_light2, 0.1f, 0.1f, 0.1f);
    LightObjectSetDiffuseColor(&spot_light2, 1.0f, 0.4f, 0.4f);
    LightObjectSetDirection(&spot_light2, 0.1f, 1, 0.1f);
    LightObjectSetPosition(&spot_light2, 0, 0.1f, 0);

    //Террайн --------
    TerrainParam tParam;
    TerrainObjectMakeDefaultParams(&tParam, TEXTURE_WIDTH, TEXTURE_WIDTH);
    tParam.t_g_param.size_factor += 20;
    tParam.t_g_param.height_factor += 30;

    tParam.textures = (char **)calloc(20, sizeof(char *));
    textures = (char **)calloc(20, sizeof(char *));

    tParam.texture_map = ToolsMakeString(path, "/textures/texture_map.png");
    AddTextureToTextures(&tParam, "/textures/wave.png");
    AddTextureToTextures(&tParam, "/textures/rock.jpg");
    AddTextureToTextures(&tParam, "/textures/snow_rock.jpg");
    AddTextureToTextures(&tParam, "/textures/grass.jpg");

    TerrainObjectDefaultInit(&terrain, &dParam, &tParam);

    some_map = calloc(TEXTURE_WIDTH * TEXTURE_HEIGHT, sizeof(uint32_t));
    TerrainObjectGenerateTerrainTextureMap(&terrain, some_map);
    free(some_map);
    GameObject3DEnableLight(&terrain, true);

    free(tParam.texture_map);
    free(tParam.textures);



    vertexParam *vParam = &terrain.go.graphObj.shapes[0].vParam;
    TerrainVertex *verts = vParam->vertices;

    TreeParams treeParams;
    treeParams.height = 35;
    treeParams.grad = 0.004f;
    treeParams.max_branches = 7;
    treeParams.mutable_rotation = vec3_f(45, 10, 25);
    treeParams.start_rotation = vec3_f(0, 0, 0);
    treeParams.radius = 2;
    treeParams.tree_color = vec3_f(0.54, 0.26, 0.074);
    treeParams.leaf_color = vec3_f(0.1, 0.3, 0.1);

    TreeObjectInitDefault(&trees[num_trees], ENGINE_TREE_OBJECT_TYPE_VERTEX, &dParam, &treeParams);
    Transform3DSetPosition(&trees[num_trees], verts[0].position.x, verts[0].position.y - 5, verts[0].position.z);
    Transform3DSetRotate(&trees[num_trees], 0, 45, 0);
    num_trees ++;

    uint32_t iter = 1, x = 0, y = 0, z = 0;
    while(iter < vParam->verticesSize)
    {
        if(verts[iter].position.y < -10.0f && verts[iter].position.y > -70.0f)
        {

            if(num_trees + 1 >= MAX_TREES)
                break;

            GameObject3DInitCopy(&trees[num_trees], &trees[0]);
            Transform3DSetPosition(&trees[num_trees], verts[iter].position.x, verts[iter].position.y - 5, verts[iter].position.z);
            Transform3DSetRotate(&trees[num_trees], 0, 45, 0);
            //Transform3DSetScale(&trees[num_trees], 0.7, 0.7, 0.7);

            num_trees ++;
        }

        x += 39;

        if(x >= tParam.size_patch){
            x = 0;
            z += 39;
        }

        iter = (x * tParam.size_patch) + z;
    }

    treeParams.grad = 0.005f;
    treeParams.height = 10;
    treeParams.max_branches = 6;
    treeParams.mutable_rotation = vec3_f(45, 10, -5);
    treeParams.start_rotation = vec3_f(0, 0, 0);
    treeParams.radius = 2;
    treeParams.leaf_color = vec3_f(0.4, 0.6, 0.4);


    TreeObjectInitDefault(&cust[0], ENGINE_TREE_OBJECT_TYPE_VERTEX, &dParam, &treeParams);
    Transform3DSetPosition(&cust[0], verts[0].position.x, verts[0].position.y, verts[0].position.z);
    Transform3DSetRotate(&cust[0], 0, 45, 0);
    Transform3DSetScale(&cust[0], 0.05, 0.05, 0.05);
    num_cust ++;

    iter = 1; x = 0; y = 0; z = 0;
    while(iter < vParam->verticesSize)
    {
        if(verts[iter].position.y < -20.0f && verts[iter].position.y > -70.0f)
        {

            if(num_cust + 1 >= MAX_CUST)
                break;

            GameObject3DInitCopy(&cust[num_cust], &cust[0]);
            Transform3DSetPosition(&cust[num_cust], verts[iter].position.x, verts[iter].position.y, verts[iter].position.z);
            Transform3DSetRotate(&cust[num_cust], 0, 45, 0);
            Transform3DSetScale(&cust[num_cust], 0.05, 0.05, 0.05);
            //Transform3DSetScale(&trees[num_trees], 0.7, 0.7, 0.7);

            num_cust ++;
        }

        x += 7;

        if(x >= tParam.size_patch){
            x = 0;
            z += 7;
        }

        iter = (x * tParam.size_patch) + z;
    }

    /*uint32_t size = 50;
    float *grid = calloc(size * size * size, sizeof(float));

    vec3 s_pos = vec3_f(40, 40, 40);
    vec3 s_pos2 = vec3_f(10, 10, 10);

    for(int x=0;x < size; x++)
    {
        for(int y=0;y < size; y++)
        {
            for(int z=0;z < size; z++)
            {
                float t = 0.1;
                float radius = 3;

                vec3 n_pos = v3_lerp(s_pos2, s_pos, t);
                while(t < 1.0)
                {
                    float dist = v3_distance(n_pos, vec3_f(x, y, z));

                    if(dist < radius)
                        MarchingCubeObjectSetGridValue(grid, size, x, y, z, 1.0);

                    t += 0.05f;
                    radius -= 0.1f;

                    n_pos = v3_lerp(s_pos2, s_pos, t);
                }
            }
        }
    }

    MarchingCubeObjectDefaultInit(&mco, grid, size, 1.0, &dParam);*/

    point_light.linear = 0.2;
    point_light.constant = 1;
    point_light.quadratic = 0.02;

    Camera3DSetPosition(0, 0, 0);
}

void SomeWalk()
{
    vertexParam *vParam = &terrain.go.graphObj.shapes[0].vParam;

    TerrainVertex *verts = vParam->vertices;

    vec3 pos = Camera3DGetPosition();

    pos.y -= 0.1f;

    float dist = 100.f;

    for(int i=0;i < vParam->verticesSize;i++)
    {
        vec3 pos2 = pos;
        pos2.y = verts[i].position.y;

        float some_dist = v3_distance(pos2, verts[i].position);

        if(some_dist < 3.0f)
        {
            dist = some_dist;

            pos.y = lerp(pos.y, verts[i].position.y + 3.f, 0.1f);

            break;
        }
    }

    LightObjectSetPosition(&point_light, pos.x, pos.y + 1, pos.z);

    Camera3DSetPosition(pos.x, pos.y, pos.z);
}

void UpdateMeshes(double deltaTime){

    Load3DglTFNextFrame(&mo, deltaTime, 0);

    SomeWalk();

    vec3 s_dir = v3_muls(light_direction, -1);
    vec3 cam_pos = Camera3DGetPosition();

    LightObjectSetDirection(&dir_light, light_direction.x, light_direction.y, light_direction.z);
    LightObjectSetPosition(&dir_light, cam_pos.x, cam_pos.y, cam_pos.z);

    LightObjectSetDirection(&spot_light, light_direction.x, light_direction.y, light_direction.z);
    LightObjectSetPosition(&spot_light, 0, 1, 0);

    //LightObjectSetPosition(&point_light, cam_pos.x, cam_pos.y, cam_pos.z);
    /*vec3 color;

    vec3 norm = { 0, 1, 0};

    float some_dot = v3_dot(norm, light_direction);

    if(some_dot < 0)
    {
        some_dot = fabs(some_dot);

        color.x = lerp(0.1, 0.5, some_dot);
        color.y = lerp(0.1, 0.4f, some_dot);
        color.z = lerp(0.1, 0.2, some_dot);
    }else{

        color.x = lerp(0.1, 0.65, some_dot);
        color.y = lerp(0.1, 0.75f, some_dot);
        color.z = lerp(0.1, 0.9, some_dot);
    }

    LightObjectSetAmbientColor(&dir_light, color.x / 100, color.y / 100, color.z / 100);
    LightObjectSetDiffuseColor(&dir_light, color.x, color.y, color.z);*/

    //TerrainObjectUpdate(&terrain);
}

void DrawMeshes(){

    LightObjectDraw(&dir_light);
    //LightObjectDraw(&point_light);
    //LightObjectDraw(&point_light2);
    //LightObjectDraw(&spot_light);
    //LightObjectDraw(&spot_light2);

    EngineDraw(&skybox);
    //EngineDraw(&mco);
    //EngineDraw(&mo);

    for(int i=0;i < num_cust;i++)
            EngineDraw(&cust[i]);

    for(int i=0;i < num_trees;i++)
            EngineDraw(&trees[i]);

    EngineDraw(&terrain);
    /*EngineDraw(&plane);
    EngineDraw(&ball);
    EngineDraw(&box);
    EngineDraw(&box2);

    EngineDraw(&f_obj);
    EngineDraw(&b_obj);
    EngineDraw(&l_obj);
    EngineDraw(&r_obj);*/

    //EngineDraw(&quad);

    /*EngineDraw(&texture_plane);
    EngineDraw(&text);*/

}

void DestroyMeshes(){
    GameObjectDestroy(&mo);
    GameObjectDestroy(&skybox);
    GameObjectDestroy(&quad);
    GameObjectDestroy(&plane);
    GameObjectDestroy(&ball);
    GameObjectDestroy(&box);
    GameObjectDestroy(&box2);

    /*GameObjectDestroy(&mco);
    free(mco.grid);*/

    for(int i=0;i < num_cust;i++)
            GameObjectDestroy(&cust[i]);

    for(int i=0;i < num_trees;i++)
            GameObjectDestroy(&trees[i]);

    GameObjectDestroy(&f_obj);
    GameObjectDestroy(&b_obj);
    GameObjectDestroy(&l_obj);
    GameObjectDestroy(&r_obj);

    GameObjectDestroy(&terrain);
    /*GameObjectDestroy(&texture_plane);
    GameObjectDestroy(&text);*/
}
