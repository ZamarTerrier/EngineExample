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

SkyObject skybox;

PrimitiveObject cube;

TreeObject tree;
TreeObject cust;

TerrainObject terrain;

LightObject dir_light;

uint32_t *some_map;

void AddTextureToTextures(TerrainParam * tParam, char * name)
{
    tParam->textures[tParam->t_t_param.num_textures] = ToolsMakeString(path, name);
    tParam->t_t_param.num_textures++;
}

void InitMeshes(){

    DrawParam dParam;
    memset(&dParam, 0, sizeof(DrawParam));

    dParam.render = &render_window;

    ToolsAddStrings(dParam.diffuse, 256, path, "/textures/texture.jpg");

    LightObjectInit(&dir_light, ENGINE_LIGHT_TYPE_DIRECTIONAL);
    LightObjectSetAmbientColor(&dir_light, 0.01f, 0.01f, 0.01f);
    LightObjectSetDiffuseColor(&dir_light, 0.09f, 0.09f, 0.09f);
    LightObjectSetDirection(&dir_light, 0.3, 0.5, 0);

    //Террайн --------
    TerrainParam tParam;
    TerrainObjectMakeDefaultParams(&tParam, TEXTURE_WIDTH, TEXTURE_WIDTH);
    tParam.vertex_step ++;
    tParam.t_g_param.size_factor += 20;
    tParam.t_g_param.height_factor += 30;

    tParam.textures = (char **)calloc(20, sizeof(char *));

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

    /*
    PrimitiveObjectInit(&cube, &dParam, ENGINE_PRIMITIVE3D_CUBE, NULL);

    GameObject3DInitInstances(&cube);


    for(int i=0;i < 50000;i++){
        cube.go.instances[i].position.x = rand() % 300;
        cube.go.instances[i].position.y = rand() % 300;
        cube.go.instances[i].position.z = rand() % 300;
        cube.go.instances[i].rotation.x = 1;
        cube.go.instances[i].rotation.y = 1;
        cube.go.instances[i].rotation.z = 1;
        cube.go.instances[i].scale = 1;
    }

    cube.go.num_instances = 50000;

    GameObject3DUpdateInstances(&cube);
    PrimitiveObjectSetInstanceDescriptor(&cube, &dParam);
    GameObject3DInitDraw(&cube);*/

    TreeParams treeParams;
    treeParams.height = 35;
    treeParams.grad = 0.004f;
    treeParams.max_branches = 6;
    treeParams.mutable_rotation = vec3_f(45, 10, 25);
    treeParams.start_rotation = vec3_f(0, 0, 0);
    treeParams.radius = 2;
    treeParams.tree_color = vec3_f(0.54, 0.26, 0.074);
    treeParams.leaf_color = vec3_f(0.1, 0.3, 0.1);

    uint32_t some_count = 1000;

    TreeObjectInit(&tree, ENGINE_TREE_OBJECT_TYPE_VERTEX, &dParam, &treeParams);
    TreeObjectInitInstances(&tree);


    vertexParam *vParam = &terrain.go.graphObj.shapes[0].vParam;
    TerrainVertex *verts = vParam->vertices;

    uint32_t iter = 0, some_break = 0;
    for(int i=0;i < some_count;i++){

        while(verts[iter].position.y > -10 || verts[iter].position.y < -100)
        {

            if(iter >= vParam->verticesSize)
            {
                some_break = 1;
                break;
            }

            iter += 73;
        }

        if(some_break)
            break;

        tree.go.instances[i].position.x = verts[iter].position.x;
        tree.go.instances[i].position.y = verts[iter].position.y - 1;
        tree.go.instances[i].position.z = verts[iter].position.z;
        tree.go.instances[i].rotation.x = 0;
        tree.go.instances[i].rotation.y = 0;
        tree.go.instances[i].rotation.z = 0;
        tree.go.instances[i].scale = 1;

        tree.go.num_instances++;

        iter +=73;
    }

    GameObject3DUpdateInstances(&tree);
    TreeObjectSetInstanceDescriptor(&tree, &dParam);
    GameObject3DInitDraw(&tree);

    treeParams.grad = 0.005f;
    treeParams.height = 8;
    treeParams.max_branches = 4;
    treeParams.mutable_rotation = vec3_f(45, 10, -5);
    treeParams.start_rotation = vec3_f(0, 0, 0);
    treeParams.radius = 2;
    treeParams.leaf_color = vec3_f(0.4, 0.6, 0.4);

    TreeObjectInit(&cust, ENGINE_TREE_OBJECT_TYPE_VERTEX, &dParam, &treeParams);
    TreeObjectInitInstances(&cust);

    some_count = 5000;
    iter = 0;
    some_break = 0;

    for(int i = 0;i < some_count;i++){

        while(verts[iter].position.y > -40 || verts[iter].position.y < -140)
        {

            if(iter >= vParam->verticesSize)
            {
                some_break = 1;
                break;
            }

            iter += 13;
        }

        if(some_break)
            break;


        cust.go.instances[i].position.x = verts[iter].position.x;
        cust.go.instances[i].position.y = verts[iter].position.y - 0.1;
        cust.go.instances[i].position.z = verts[iter].position.z;
        cust.go.instances[i].rotation.x = 0;
        cust.go.instances[i].rotation.y = 0;
        cust.go.instances[i].rotation.z = 0;
        cust.go.instances[i].scale = 0.2;

        cust.go.num_instances++;

        iter +=17;
    }

    GameObject3DUpdateInstances(&cust);
    TreeObjectSetInstanceDescriptor(&cust, &dParam);
    GameObject3DInitDraw(&cust);

    SkyObjectInit(&skybox, &dParam, ENGINE_SKY_TYPE_ATMOSPHERIC);
    SkyObjectAddDefault(&skybox, &render_window);
    GameObject2DInitDraw(&skybox);

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

    //LightObjectSetPosition(&point_light, pos.x, pos.y + 1, pos.z);

    Camera3DSetPosition(pos.x, pos.y, pos.z);
}

void UpdateMeshes(double deltaTime){

    SomeWalk();

    vec3 cam_pos = Camera3DGetPosition();

    LightObjectSetDirection(&dir_light, light_direction.x, light_direction.y, light_direction.z);
    LightObjectSetPosition(&dir_light, cam_pos.x, cam_pos.y, cam_pos.z);

}

void DrawMeshes(){
    EngineDraw(&skybox);

    LightObjectDraw(&dir_light);

    EngineDraw(&terrain);
    EngineDraw(&tree);
    EngineDraw(&cust);
}

void DestroyMeshes(){
    GameObjectDestroy(&skybox);

    GameObjectDestroy(&terrain);
    GameObjectDestroy(&tree);
    GameObjectDestroy(&cust);
}
