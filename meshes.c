#include "meshes.h"

#include <stdlib.h>
#include <time.h>

#include "engine.h"

#include "resource.h"

#include "e_blue_print.h"

#include "gameObject2D.h"
#include "gameObject3D.h"

#include "sky_object.h"
#include "quad_object.h"
#include "primitiveObject.h"
#include "water_object.h"
#include "terrain_object.h"
#include "tree_object.h"
#include "grass_object.h"

#include "e_transform.h"

#include "e_math.h"

#include "e_resource_data.h"

#define HEIGHT_MAP_SIZE 4096

SkyObject skydome;

TerrainObject terrain;

TreeObject tree_mesh;
TreeObject cust_mesh;

WaterObject water;

PrimitiveObject cube;

QuadObject img_quad;

GrassObject grass_mesh;
GrassObject grass2_mesh;

void *my_map;

uint32_t size_terrain = 400;

const float coeff = 5.85;
const float resist = 4.15f;

void MeshesAddTextureToTextures(TerrainParam * tParam, char * name)
{
    tParam->textures[tParam->t_t_param.num_textures] = ToolsMakeString(path, name);
    tParam->t_t_param.num_textures++;
}

void InitMeshes(){
    DrawParam dParam;
    memset(&dParam, 0, sizeof(DrawParam));

    dParam.render = &render_window;

    //------------------------------
    {
        /*ToolsAddStrings(dParam.diffuse, 256, path, "/textures/texture.jpg");
        PrimitiveObjectInit(&cube, &dParam, ENGINE_PRIMITIVE3D_CUBE, NULL);
        GameObject3DInitInstances(&cube);

        for(int i=0;i < UINT16_MAX;i++){
            cube.go.instances[i].position.x = rand() % 1000;
            cube.go.instances[i].position.y = rand() % 1000;
            cube.go.instances[i].position.z = rand() % 1000;
            cube.go.instances[i].rotation.x = 0;
            cube.go.instances[i].rotation.y = 0;
            cube.go.instances[i].rotation.z = 0;
            cube.go.instances[i].scale = 1;
        }

        cube.go.num_instances = UINT16_MAX;

        GameObject3DUpdateInstances(&cube);
        PrimitiveObjectSetInstanceDescriptor(&cube, &dParam);
        GameObject3DInitDraw(&cube);*/
    }

    //Террайн-----------------------------------------------------------
    TerrainParam tParam;
    TerrainObjectMakeDefaultParams(&tParam, TEXTURE_MAP_SIZE, TEXTURE_MAP_SIZE, HEIGHT_MAP_SIZE);

    //tParam.flags &= ~(ENGINE_TERRIAN_FLAGS_GENERATE_HEIGHTS);
    //tParam.flags |= ENGINE_TERRIAN_FLAGS_GENERATE_HEIGHTS_OLD;
    //tParam.flags |= ENGINE_TERRIAN_FLAGS_GENERATE_HEIGHTS_PERLIN;

    tParam.size_patch = size_terrain;

    tParam.textures = (char **)calloc(20, sizeof(char *));

    tParam.texture_map = ToolsMakeString(path, "/textures/texture_map.png");
    MeshesAddTextureToTextures(&tParam, "/textures/sand.jpg");
    MeshesAddTextureToTextures(&tParam, "/textures/rock.jpg");
    MeshesAddTextureToTextures(&tParam, "/textures/snow_rock.jpg");
    MeshesAddTextureToTextures(&tParam, "/textures/grass.jpg");

    TerrainObjectDefaultInit(&terrain, &dParam, &tParam);

    my_map = calloc(TEXTURE_MAP_SIZE * TEXTURE_MAP_SIZE, sizeof(uint16_t));
    TerrainObjectGenerateTerrainTextureMap(&terrain, my_map);
    free(my_map);
    TerrainObjectGenerateTerrainHeightTextureMap(&terrain);
    //GameObject3DEnableLight(&terrain, true);

    free(tParam.texture_map);
    free(tParam.textures);

    VertexInstance3D vertex;
    vertex.rotation = vec3_f( 0, 0, 0);
    vertex.scale = 1;

    //Скопление тигрибов-----------------------------------------------
    {
        /*ToolsAddStrings(dParam.diffuse, 256, path, "/textures/texture.jpg");
        PrimitiveObjectInit(&cube, &dParam, ENGINE_PRIMITIVE3D_CUBE, NULL);
        GameObject3DInitInstances(&cube);
        for(float x = 0; x < size_terrain;x++)
        {
            for(float z = 0; z < size_terrain;z++)
            {
                vertex.position.x = (x - 4.0f - size_terrain / 2.0) * 3.92f;
                vertex.position.y = -TerrainObjectGetHeight(&terrain, x, z);
                vertex.position.z = (z - 4.0f - size_terrain / 2.0) * 3.92f;

                GameObject3DAddInstance(&cube, vertex);
            }
        }

        GameObject3DUpdateInstances(&cube);
        PrimitiveObjectSetInstanceDescriptor(&cube, &dParam);
        GameObject3DInitDraw(&cube);
        Transform3DSetPosition(&cube, 0, 0, 0);*/
    }

    //Деревья----------------------------------------
    {
        vertex.scale = 1.0;
        vertex.rotation = vec3_f(0 ,0, 0);

        TreeParams treeParams;
        treeParams.height = 35;
        treeParams.grad = 0.004f;
        treeParams.max_branches = 7;
        treeParams.mutable_rotation = vec3_f(45, 10, 35);
        treeParams.start_rotation = vec3_f(0, 0, 0);
        treeParams.radius = 4;
        treeParams.tree_color = vec3_f(0.54, 0.26, 0.074);
        treeParams.leaf_color = vec3_f(0.1, 0.3, 0.1);


        TreeObjectInit(&tree_mesh, ENGINE_TREE_OBJECT_TYPE_VERTEX, &dParam, &treeParams);
        TreeObjectInitInstances(&tree_mesh);

        float height = 0;
        bool some_break = false;
        for(float x = 0; x < size_terrain;x++)
        {
            some_break = false;
            for(float z = 0; z < size_terrain;z++)
            {
                height = -TerrainObjectGetHeight(&terrain, x, z);

                while(height > -240 || height < -370)
                {
                    z += rand() % 80;

                    if(z >= size_terrain)
                    {
                        some_break = true;
                        break;
                    }

                    height = -TerrainObjectGetHeight(&terrain, x, z);
                }

                if(some_break)
                    break;

                vertex.position.x = (x - resist - size_terrain / 2.0) * coeff;
                vertex.position.y = height + 2;
                vertex.position.z = (z - resist - size_terrain / 2.0) * coeff;
                vertex.rotation.y = (float)(random() % 100) / 100;

                GameObject3DAddInstance(&tree_mesh, vertex);

                z += rand() % 80;
            }

            x += rand() % 10;
        }

        GameObject3DUpdateInstances(&tree_mesh);
        TreeObjectSetInstanceDescriptor(&tree_mesh, &dParam);
        GameObject3DInitDraw(&tree_mesh);
    }

    //Кусты--------------------------------------------------
    {
        TreeParams treeParams;
        treeParams.grad = 0.005f;
        treeParams.height = 8;
        treeParams.max_branches = 5;
        treeParams.mutable_rotation = vec3_f(45, 10, -5);
        treeParams.start_rotation = vec3_f(0, 0, 0);
        treeParams.radius = 2;
        treeParams.leaf_color = vec3_f(0.4, 0.1, 0.2);

        TreeObjectInit(&cust_mesh, ENGINE_TREE_OBJECT_TYPE_VERTEX, &dParam, &treeParams);
        TreeObjectInitInstances(&cust_mesh);


        vertex.scale = 0.2;
        float height = 0;
        bool some_break = false;
        for(float x = 0; x < size_terrain;x++)
        {
            some_break = false;
            for(float z = 0; z < size_terrain;z++)
            {
                height = -TerrainObjectGetHeight(&terrain, x, z);

                while(height > -240 || height < -370)
                {
                    z += rand() % 50;

                    if(z >= size_terrain)
                    {
                        some_break = true;
                        break;
                    }

                    height = -TerrainObjectGetHeight(&terrain, x, z);
                }

                if(some_break)
                    break;

                vertex.position.x = (x - resist - size_terrain / 2.0) * coeff;
                vertex.position.y = height + 2;
                vertex.position.z = (z - resist - size_terrain / 2.0) * coeff;
                vertex.rotation.y = (float)(random() % 100) / 100;

                GameObject3DAddInstance(&cust_mesh, vertex);

                z += rand() % 50;
            }

            x += rand() % 5;
        }

        GameObject3DUpdateInstances(&cust_mesh);
        TreeObjectSetInstanceDescriptor(&cust_mesh, &dParam);
        GameObject3DInitDraw(&cust_mesh);
    }

    //Трава---------------------------------------
    {
        ToolsAddStrings(dParam.diffuse, 256, path, "/textures/grass.png");
        GrassObjectInit(&grass_mesh, &dParam);
        GameObject3DInitInstances(&grass_mesh);

        vertex.scale = 3.0;
        float height = 0;
        bool some_break = false;
        for(float x = 0; x < size_terrain;x++)
        {
            some_break = false;
            for(float z = 0; z < size_terrain;z++)
            {
                height = -TerrainObjectGetHeight(&terrain, x, z);

                while(height > -235 || height < -380)
                {
                    z += rand() % 2;

                    if(z >= size_terrain)
                    {
                        some_break = true;
                        break;
                    }

                    height = -TerrainObjectGetHeight(&terrain, x, z);
                }

                if(some_break)
                    break;

                vertex.position.x = (x - resist - size_terrain / 2.0) * coeff;
                vertex.position.y = height + 4;
                vertex.position.z = (z - resist - size_terrain / 2.0) * coeff;
                vertex.rotation.y = (float)(random() % 100) / 100;

                GameObject3DAddInstance(&grass_mesh, vertex);

                z += rand() % 2;
            }

            x += rand() % 2;
        }

        GameObject3DUpdateInstances(&grass_mesh);
        GrassObjectSetInstanceDescriptor(&grass_mesh, &dParam);
        GameObject3DInitDraw(&grass_mesh);

        ToolsAddStrings(dParam.diffuse, 256, path, "/textures/grass2.png");
        GrassObjectInit(&grass2_mesh, &dParam);
        GameObject3DInitInstances(&grass2_mesh);

        for(float x = 0; x < size_terrain;x++)
        {
            some_break = false;
            for(float z = 0; z < size_terrain;z++)
            {
                height = -TerrainObjectGetHeight(&terrain, x, z);

                while(height > -235 || height < -380)
                {
                    z += rand() % 2;

                    if(z >= size_terrain)
                    {
                        some_break = true;
                        break;
                    }

                    height = -TerrainObjectGetHeight(&terrain, x, z);
                }

                if(some_break)
                    break;

                vertex.position.x = (x - resist - size_terrain / 2.0) * coeff;
                vertex.position.y = height + 4;
                vertex.position.z = (z - resist - size_terrain / 2.0) * coeff;
                vertex.rotation.y = (float)(random() % 100) / 100;

                GameObject3DAddInstance(&grass2_mesh, vertex);

                z += rand() % 2;
            }

            x += rand() % 2;
        }

        GameObject3DUpdateInstances(&grass2_mesh);
        GrassObjectSetInstanceDescriptor(&grass2_mesh, &dParam);
        GameObject3DInitDraw(&grass2_mesh);
    }

    //----------------------------------------------------------------

    QuadObjectInit(&img_quad, ENGINE_QUAD_TYPE_DEPTH);
    QuadObjectAddDefault(&img_quad, dParam.render);
    BluePrintAddExistTextureImage(&img_quad.go.graphObj.blueprints, 0, &terrain.go.graphObj.blueprints.blue_print_packs[0].descriptors[7].textures[0]);
    GameObject2DInitDraw(&img_quad);
    Transform2DSetScale(&img_quad, 600, 600);
    Transform2DSetPosition(&img_quad, -200, -100);


    //Водичка----------------------------------------------------------
    ToolsAddStrings(dParam.diffuse, 256, path, "/textures/water.jpg");
    WaterObjectInitDefault(&water, &dParam, 1000);
    Transform3DSetPosition(&water, 0, -380, 0);


    //Небо---------------------------------------------------------------
    SkyObjectInit(&skydome, &dParam, ENGINE_SKY_TYPE_DAY);
    SkyObjectAddDefault(&skydome, &render_window);
    GameObject2DInitDraw(&skydome);

    Camera3DSetPosition(0, -200, 0);
}

void MeshesSomeWalk()
{

    vec3 pos = Camera3DGetPosition();

    float del = 512.0f;
    uint32_t some_x = ceil(pos.x / coeff * del) / del + (size_terrain / 2.0 + resist);
    uint32_t some_z = ceil(pos.z / coeff * del) / del + (size_terrain / 2.0 + resist);

    float some_val = -TerrainObjectGetHeight(&terrain, some_x, some_z);

    pos.y = lerp(pos.y, some_val + 10, 0.1f);

    //LightObjectSetPosition(&point_light, pos.x, pos.y + 1, pos.z);

    Camera3DSetPosition(pos.x, pos.y, pos.z);
}

void UpdateMeshes(double deltaTime){
    MeshesSomeWalk();
}

void DrawMeshes(){
    EngineDraw(&skydome);

    EngineDraw(&terrain);
    EngineDraw(&water);

    //EngineDraw(&cube);
    EngineDraw(&tree_mesh);
    EngineDraw(&cust_mesh);
    EngineDraw(&grass_mesh);
    EngineDraw(&grass2_mesh);

    EngineDraw(&img_quad);
}

void DestroyMeshes(){
    GameObjectDestroy(&skydome);


    GameObjectDestroy(&terrain);
    GameObjectDestroy(&water);

    //GameObjectDestroy(&cube);
    GameObjectDestroy(&tree_mesh);
    GameObjectDestroy(&cust_mesh);
    GameObjectDestroy(&grass_mesh);
    GameObjectDestroy(&grass2_mesh);

    GameObjectDestroy(&img_quad);
}
