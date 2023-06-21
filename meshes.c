#include "meshes.h"

#include <stdlib.h>
#include <time.h>

#include "engine.h"

#include "resource.h"

#include "gameObject2D.h"
#include "gameObject3D.h"

#include "sky_object.h"
#include "primitiveObject.h"
#include "water_object.h"
#include "terrain_object.h"

#include "e_transform.h"

#include "e_math.h"

SkyObject skydome;

TerrainObject terrain;

WaterObject water;

PrimitiveObject cube;

uint32_t *my_map;

void MeshesAddTextureToTextures(TerrainParam * tParam, char * name)
{
    tParam->textures[tParam->t_t_param.num_textures] = ToolsMakeString(path, name);
    tParam->t_t_param.num_textures++;
}

void InitMeshes(){
    DrawParam dParam;
    memset(&dParam, 0, sizeof(DrawParam));

    dParam.render = &render_window;


    //Скопление тигрибов-----------------------------------------------
    {
        ToolsAddStrings(dParam.diffuse, 256, path, "/textures/texture.jpg");
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
        GameObject3DInitDraw(&cube);
    }

    //Террайн-----------------------------------------------------------
    TerrainParam tParam;
    TerrainObjectMakeDefaultParams(&tParam, TEXTURE_WIDTH, TEXTURE_HEIGHT, 1024);

    //tParam.flags |= ENGINE_TERRIAN_FLAGS_GENERATE_HEIGHTS_OLD;

    tParam.textures = (char **)calloc(20, sizeof(char *));

    tParam.texture_map = ToolsMakeString(path, "/textures/texture_map.png");
    MeshesAddTextureToTextures(&tParam, "/textures/wave.png");
    MeshesAddTextureToTextures(&tParam, "/textures/rock.jpg");
    MeshesAddTextureToTextures(&tParam, "/textures/snow_rock.jpg");
    MeshesAddTextureToTextures(&tParam, "/textures/grass.jpg");

    TerrainObjectDefaultInit(&terrain, &dParam, &tParam);

    my_map = calloc(TEXTURE_WIDTH * TEXTURE_HEIGHT, sizeof(uint32_t));
    TerrainObjectGenerateTerrainTextureMap(&terrain, my_map);
    TerrainObjectGenerateTerrainHeightTextureMap(&terrain, my_map);
    free(my_map);
    //GameObject3DEnableLight(&terrain, true);

    free(tParam.texture_map);
    free(tParam.textures);

    //Водичка----------------------------------------------------------
    ToolsAddStrings(dParam.diffuse, 256, path, "/textures/water.jpg");
    WaterObjectInitDefault(&water, &dParam, 1000);
    Transform3DSetPosition(&water, 0, -188, 0);


    //Небо---------------------------------------------------------------
    SkyObjectInit(&skydome, &dParam, ENGINE_SKY_TYPE_NIGHT);
    SkyObjectAddDefault(&skydome, &render_window);
    GameObject2DInitDraw(&skydome);

    Camera3DSetPosition(0, 0, 0);
}

void MeshesSomeWalk()
{

    vec3 pos = Camera3DGetPosition();

    float scale = terrain.t_t_param.height_map_scale;

    if(pos.x <= -scale / 2 || pos.x >= scale / 2)
        return;

    if(pos.z <= -scale / 2|| pos.z >= scale / 2)
        return;

    char *heights = terrain.height_map;

    pos.y -= 0.1f;

    float c_x = floor(pos.x  * 64) / 64;
    float c_z = floor(pos.z  * 64) / 64;

    int iter = ceil(c_z + scale / 2) * scale + ceil(c_x+ scale / 2);

    float some_val = heights[iter];

    pos.y = lerp(pos.y, -some_val + 1, 0.1f);

    //LightObjectSetPosition(&point_light, pos.x, pos.y + 1, pos.z);

    Camera3DSetPosition(pos.x, pos.y, pos.z);
}

void UpdateMeshes(double deltaTime){
    //MeshesSomeWalk();
}

void DrawMeshes(){
    EngineDraw(&skydome);

    /*EngineDraw(&terrain);
    EngineDraw(&water);*/

    EngineDraw(&cube);
}

void DestroyMeshes(){
    GameObjectDestroy(&skydome);

    GameObjectDestroy(&terrain);
    GameObjectDestroy(&water);

    GameObjectDestroy(&cube);
}
