#include "landscape.h"

#include "engine.h"

#include "e_camera.h"

#include "gameObject.h"
#include "gameObject2D.h"
#include "gameObject3D.h"

#include "sky_object.h"
#include "primitiveObject.h"
#include "terrain_object.h"
#include "lightObject.h"
#include "tree_object.h"
#include "grass_object.h"
#include "water_object.h"

#include "resource.h"

#include "e_math.h"

SkyObject skybox;

WaterObject wave;

TreeObject tree;
TreeObject cust;

GrassObject grass;
GrassObject grass2;

TerrainObject land;

LightObject dir_light;

uint32_t *some_map;

void AddTextureToTextures(TerrainParam * tParam, char * name)
{
    tParam->textures[tParam->t_t_param.num_textures] = ToolsMakeString(path, name);
    tParam->t_t_param.num_textures++;
}

void InitLandscape(){
    DrawParam dParam;
    memset(&dParam, 0, sizeof(DrawParam));

    dParam.render = &render_window;

    ToolsAddStrings(dParam.diffuse, 256, path, "/textures/texture.png");

    //Освещение-------------------------------------------------
    {
        LightObjectInit(&dir_light, ENGINE_LIGHT_TYPE_DIRECTIONAL);
        LightObjectSetAmbientColor(&dir_light, 0.01f, 0.01f, 0.01f);
        LightObjectSetDiffuseColor(&dir_light, 0.09f, 0.09f, 0.09f);
        LightObjectSetDirection(&dir_light, 0.3, 0.5, 0);
    }
    //Террайн -----------------------------------------------
    {
        TerrainParam tParam;
        TerrainObjectMakeOldDefaultParams(&tParam, TEXTURE_MAP_SIZE, TEXTURE_MAP_SIZE, 1024);
        tParam.vertex_step += 1.0f;
        tParam.t_g_param.octaves ++;
        tParam.t_g_param.size_factor += 50;
        tParam.t_g_param.height_factor += 35;
        tParam.flags |= ENGINE_TERRIAN_FLAGS_GENERATE_HEIGHTS_OLD;

        tParam.textures = (char **)calloc(20, sizeof(char *));

        tParam.texture_map = ToolsMakeString(path, "/textures/texture_map.png");
        AddTextureToTextures(&tParam, "/textures/sand.jpg");
        AddTextureToTextures(&tParam, "/textures/rock.jpg");
        AddTextureToTextures(&tParam, "/textures/snow_rock.jpg");
        AddTextureToTextures(&tParam, "/textures/grass.jpg");

        TerrainObjectDefaultInit(&land, &dParam, &tParam);

        some_map = calloc(TEXTURE_MAP_SIZE * TEXTURE_MAP_SIZE, sizeof(uint16_t));
        TerrainObjectGenerateTerrainTextureMap(&land, some_map);
        free(some_map);
        GameObject3DEnableLight(&land, true);

        free(tParam.texture_map);
        free(tParam.textures);
    }

    //Водичка ---------------------------------------------------
    {
        ToolsAddStrings(dParam.diffuse, 256, path, "/textures/water.jpg");
        WaterObjectInitDefault(&wave, &dParam, 1000);
        Transform3DSetPosition(&wave, 0, -160, 0);
    }

    vertexParam *vParam = &land.go.graphObj.shapes[0].vParam;
    TerrainVertex *verts = vParam->vertices;

    VertexInstance3D vertex;
    vertex.scale = 1;
    vertex.rotation = vec3_f(0 ,0, 0);

    uint32_t some_count;
    uint32_t iter = 0, some_break = 0;
    //Трава---------------------------------------
    {
        ToolsAddStrings(dParam.diffuse, 256, path, "/textures/grass.png");
        GrassObjectInit(&grass, &dParam);
        GameObject3DInitInstances(&grass);

        some_count = 40000;
        iter = 0;
        some_break = 0;

        vertex.scale = 3.0;
        for(int i = 0;i < some_count;i++){

            if(iter >= vParam->verticesSize)
                break;

            while(verts[iter].position.y > -10 || verts[iter].position.y < -140)
            {
                iter += rand() % 3 + 7;

                if(iter >= vParam->verticesSize)
                {
                    some_break = 1;
                    break;
                }
            }

            if(some_break)
                break;

            vertex.position = verts[iter].position;
            vertex.position.y += vertex.scale;

            GameObject3DAddInstance(&grass, vertex);

            iter += rand() % 3 + 7;
        }

        GameObject3DUpdateInstances(&grass);
        GrassObjectSetInstanceDescriptor(&grass, &dParam);
        GameObject3DInitDraw(&grass);

        ToolsAddStrings(dParam.diffuse, 256, path, "/textures/grass2.png");
        GrassObjectInit(&grass2, &dParam);
        GameObject3DInitInstances(&grass2);

        some_count = 40000;
        iter = 0;
        some_break = 0;

        vertex.scale = 3.0;
        for(int i = 0;i < some_count;i++){

            if(iter >= vParam->verticesSize)
                break;

            while(verts[iter].position.y > -10 || verts[iter].position.y < -140)
            {
                iter += rand() % 3 + 3;

                if(iter >= vParam->verticesSize)
                {
                    some_break = 1;
                    break;
                }
            }

            if(some_break)
                break;

            vertex.position = verts[iter].position;
            vertex.position.y += 1;

            vertex.rotation.y = (float)(random() % 100) / 100;

            GameObject3DAddInstance(&grass2, vertex);

            iter += rand() % 3 + 3;
        }

        GameObject3DUpdateInstances(&grass2);
        GrassObjectSetInstanceDescriptor(&grass2, &dParam);
        GameObject3DInitDraw(&grass2);
    }
    //Деревья----------------------------------------
    {
        some_count = 500;
        iter = 0;
        some_break = 0;

        vertex.scale = 1.0;
        vertex.rotation = vec3_f(0 ,0, 0);

        TreeParams treeParams;
        treeParams.height = 35;
        treeParams.grad = 0.004f;
        treeParams.max_branches = 7;
        treeParams.mutable_rotation = vec3_f(45, 10, 25);
        treeParams.start_rotation = vec3_f(0, 0, 0);
        treeParams.radius = 2;
        treeParams.tree_color = vec3_f(0.54, 0.26, 0.074);
        treeParams.leaf_color = vec3_f(0.1, 0.3, 0.1);


        TreeObjectInit(&tree, ENGINE_TREE_OBJECT_TYPE_VERTEX, &dParam, &treeParams);
        TreeObjectInitInstances(&tree);

        for(int i=0;i < some_count;i++){

            if(iter >= vParam->verticesSize)
                break;

            while(verts[iter].position.y > -10 || verts[iter].position.y < -110)
            {
                iter += rand() % 100  + 130;


                if(iter >= vParam->verticesSize)
                {
                    some_break = 1;
                    break;
                }
            }

            if(some_break)
                break;

            vertex.position.x = verts[iter].position.x;
            vertex.position.y = verts[iter].position.y - 1;
            vertex.position.z = verts[iter].position.z;

            GameObject3DAddInstance(&tree, vertex);

            iter += rand() % 100  + 130;
        }

        GameObject3DUpdateInstances(&tree);
        TreeObjectSetInstanceDescriptor(&tree, &dParam);
        GameObject3DInitDraw(&tree);
    }
    //Кусты--------------------------------------------------
    {
        TreeParams treeParams;
        treeParams.grad = 0.005f;
        treeParams.height = 8;
        treeParams.max_branches = 4;
        treeParams.mutable_rotation = vec3_f(45, 10, -5);
        treeParams.start_rotation = vec3_f(0, 0, 0);
        treeParams.radius = 2;
        treeParams.leaf_color = vec3_f(0.4, 0.1, 0.2);

        TreeObjectInit(&cust, ENGINE_TREE_OBJECT_TYPE_VERTEX, &dParam, &treeParams);
        TreeObjectInitInstances(&cust);

        some_count = 3000;
        iter = 0;
        some_break = 0;

        vertex.scale = 0.2;
        for(int i = 0;i < some_count;i++){

            if(iter >= vParam->verticesSize)
                break;

            while(verts[iter].position.y > -40 || verts[iter].position.y < -140)
            {
                iter += rand() % 50  + 17;

                if(iter >= vParam->verticesSize)
                {
                    some_break = 1;
                    break;
                }
            }

            if(some_break)
                break;


            vertex.position.x = verts[iter].position.x;
            vertex.position.y = verts[iter].position.y - 0.1;
            vertex.position.z = verts[iter].position.z;

            GameObject3DAddInstance(&cust, vertex);

            iter += rand() % 50  + 17;
        }

        GameObject3DUpdateInstances(&cust);
        TreeObjectSetInstanceDescriptor(&cust, &dParam);
        GameObject3DInitDraw(&cust);
    }

    //Небо---------------------------------------------------------------
    SkyObjectInit(&skybox, &dParam, ENGINE_SKY_TYPE_DAY);
    SkyObjectAddDefault(&skybox, &render_window);
    GameObject2DInitDraw(&skybox);

    Camera3DSetPosition(0, 0, 0);
}

void SomeWalk()
{
    vertexParam *vParam = &land.go.graphObj.shapes[0].vParam;

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

            pos.y = lerp(pos.y, verts[i].position.y + 7.f, 0.1f);

            break;
        }
    }

    //LightObjectSetPosition(&point_light, pos.x, pos.y + 1, pos.z);

    Camera3DSetPosition(pos.x, pos.y, pos.z);
}

void UpdateLandscape(double deltaTime){

    SomeWalk();

    vec3 cam_pos = Camera3DGetPosition();

    LightObjectSetDirection(&dir_light, light_direction.x, light_direction.y, light_direction.z);
    LightObjectSetPosition(&dir_light, cam_pos.x, cam_pos.y, cam_pos.z);


    /*for(int i = 0;i < tree.go.num_instances;i++){
        tree.go.instances[i].rotation.x += 0.01;
        tree.go.instances[i].rotation.y += 0.01;
        tree.go.instances[i].rotation.z += 0.01;

    }*/

    GameObject3DUpdateInstances(&tree);
}

void DrawLandscape(){
    EngineDraw(&skybox);

    LightObjectDraw(&dir_light);

    EngineDraw(&land);
    EngineDraw(&tree);
    EngineDraw(&cust);
    EngineDraw(&grass);
    EngineDraw(&grass2);
    EngineDraw(&wave);

    //EngineDraw(&cube);
}

void DestroyLandscape(){
    GameObjectDestroy(&skybox);

    GameObjectDestroy(&land);
    GameObjectDestroy(&tree);
    GameObjectDestroy(&cust);
    GameObjectDestroy(&grass);
    GameObjectDestroy(&grass2);
    GameObjectDestroy(&wave);

    //GameObjectDestroy(&cube);
}
