#include "physics.h"

#include "engine.h"
#include "textObject.h"

#include "resource.h"

#include "e_tools.h"

#define MAX_PHYOBJECTS 128

#include <ccd/ccd.h>

typedef struct{
    PhysObj phyObjects[MAX_PHYOBJECTS];
    int phyObjectsCount;
} PysicsStruct;

struct Sphere
{
    vec3 m_Center;
    float m_Radius;
};

PysicsStruct pysics;

float worldStep = 0;

float worldSpeed = 0.001f;

float worldGravity = 10.0f;

vec3 moveDirGrav = {0, -1, 0};

ccd_t ccd;

void SphereSupport(void *obj, const ccd_vec3_t *dir, ccd_vec3_t *vec){
    InterSphereParam *sph = obj;

    vec3 _dir;

    _dir.x = dir->v[0];
    _dir.y = dir->v[1];
    _dir.z = dir->v[2];

    vec3 new = v3_norm(_dir);

    vec->v[0] = new.x * sph->radius + sph->center.x;
    vec->v[1] = new.y * sph->radius + sph->center.y;
    vec->v[2] = new.z * sph->radius + sph->center.z;
}

vec3 PhyGetPos(PhysObj *obj){

    InterSphereParam *sphParam = (InterSphereParam *) obj->params.obj;
    InterAABBParam *boxParam = (InterAABBParam *) obj->params.obj;
    InterCapsuleParam *capParam = (InterCapsuleParam *) obj->params.obj;

    switch(obj->collType)
    {
        case PHYSICS_COLLIDER_TYPE_SPHERE:
            return sphParam->center;
            break;
        case PHYSICS_COLLIDER_TYPE_BOX:
            return boxParam->position;
            break;
        case PHYSICS_COLLIDER_TYPE_CAPSULE:
            return capParam->position;
            break;
        case PHYSICS_COLLIDER_TYPE_MESH:
            return Transform3DGetPosition(obj->params.obj);
            break;
    }

    return (vec3){ 0, 0, 0};
}

void PhySetPos(PhysObj *obj, vec3 pos){

    InterSphereParam *sphParam = (InterSphereParam *) obj->params.obj;
    InterAABBParam *boxParam = (InterAABBParam *) obj->params.obj;
    InterCapsuleParam *capParam = (InterCapsuleParam *) obj->params.obj;

    switch(obj->collType)
    {
        case PHYSICS_COLLIDER_TYPE_SPHERE:
            sphParam->center = pos;
            break;
        case PHYSICS_COLLIDER_TYPE_BOX:
            boxParam->position = pos;
            break;
        case PHYSICS_COLLIDER_TYPE_CAPSULE:
            capParam->position = pos;
            break;
        case PHYSICS_COLLIDER_TYPE_MESH:
            Transform3DSetPosition(obj->params.obj, pos.x, pos.y, pos.z);
            break;
    }
}

int TestCollisions(int iter){

    int res = 0, buff = 0;
    float distance = 0, depth = 0;

    vec3 dir;

    PhysObj *act = &pysics.phyObjects[iter];

    InterSphereParam *sphParam = act->params.obj;
    InterAABBParam *cubeParam = act->params.obj;

    ccd_real_t c_depth;
    ccd_vec3_t c_dir, c_pos;

    for(int j=0; j < pysics.phyObjectsCount; j++){

        if(iter == j)
            continue;

        PhysObj *refl = &pysics.phyObjects[j];

        if(act->collType == PHYSICS_COLLIDER_TYPE_SPHERE)
        {
            switch(refl->collType)
            {
                case PHYSICS_COLLIDER_TYPE_PLANE:
                    buff = IntersectionSpherePlane(refl->params.obj, act->params.obj, &distance, &depth, &dir);
                    break;
                case PHYSICS_COLLIDER_TYPE_SPHERE:
                    buff = !ccdGJKPenetration(act->params.obj, refl->params.obj, &ccd, &c_depth, &c_dir, &c_pos) ? 1 : 0;
                    depth = c_depth;
                    dir.x = c_dir.v[0];
                    dir.y = c_dir.v[1];
                    dir.z = c_dir.v[2];
                    //buff = IntersectionSphereToSphere(act->params.obj, refl->params.obj, &distance, &depth, &dir);
                    break;                    
                case PHYSICS_COLLIDER_TYPE_BOX:
                    buff = IntersectionSphereAABB(act->params.obj, refl->params.obj, &distance, &depth, &dir);
                    break;
                case PHYSICS_COLLIDER_TYPE_MESH:
                    buff = IntersectionSphereShape(act->params.obj, refl->params.obj, &distance, &depth, &dir);
                    break;
                case PHYSICS_COLLIDER_TYPE_CAPSULE:
                    buff = IntersectionSphereCapsule(act->params.obj, refl->params.obj, &distance, &depth, &dir);
                    break;
                case PHYSICS_COLLIDER_TYPE_TERRAIN:
                    buff = IntersectionSphereTerrain(act->params.obj, refl->params.obj, &distance, &depth, &dir);
                    break;
                default:
                    break;
            }
        }else if(act->collType == PHYSICS_COLLIDER_TYPE_CAPSULE)
        {
            switch(refl->collType)
            {
                case PHYSICS_COLLIDER_TYPE_SPHERE:
                    buff = IntersectionSphereCapsule(refl->params.obj, act->params.obj, &distance, &depth, &dir);
                    break;
                case PHYSICS_COLLIDER_TYPE_MESH:
                    buff = IntersectionCapsuleShape(act->params.obj, refl->params.obj, &distance, &depth, &dir);
                    break;
                case PHYSICS_COLLIDER_TYPE_CAPSULE:
                    buff = IntersectionCapsuleCapsule(act->params.obj, refl->params.obj, &distance, &depth, &dir);
                    break;
                case PHYSICS_COLLIDER_TYPE_BOX:
                    buff = IntersectionAABBCapsule(act->params.obj, refl->params.obj, &distance, &depth, &dir);
                    break;
                default:
                    break;
            }
        }else if(act->collType == PHYSICS_COLLIDER_TYPE_BOX)
        {
            switch(refl->collType)
            {

                case PHYSICS_COLLIDER_TYPE_PLANE:
                    buff = IntersectionAABBPlane(act->params.obj, refl->params.obj, &distance, &depth, &dir);
                    break;
                case PHYSICS_COLLIDER_TYPE_SPHERE:
                    buff = IntersectionSphereAABB(refl->params.obj, act->params.obj, &distance, &depth, &dir);
                    break;
                case PHYSICS_COLLIDER_TYPE_MESH:
                    //buff = IntersectionSphereShape(act->params.obj, refl->params.obj, &distance, &depth, &dir);
                    break;
                case PHYSICS_COLLIDER_TYPE_CAPSULE:
                    buff = IntersectionAABBCapsule(refl->params.obj, act->params.obj, &distance, &depth, &dir);
                    break;
                case PHYSICS_COLLIDER_TYPE_BOX:
                    buff = IntersectionAABBAABB(act->params.obj, refl->params.obj, &distance, &depth, &dir);
                    break;
                default:
                    break;
            }

            depth = cubeParam->size / 2;
        }

        if(refl->collType == PHYSICS_COLLIDER_TYPE_TERRAIN && buff)
        {
            act->params.moveDir.x = 0;
            act->params.moveDir.y = 0;
            act->params.moveDir.z = 0;

            return buff;
        }

        vec3 dd = v3_muls(dir, -depth);

        if(act->physType == PHYSICS_PHYSIC_DINAMIC && buff)
        {

            if(refl->physType == PHYSICS_PHYSIC_DINAMIC)
            {

                act->params.moveDir = v3_add(v3_muls(act->params.moveDir, 0.5f), dd);

            }else
                act->params.moveDir = v3_add(v3_muls(act->params.moveDir, 0.5f), dd);

        }

        if(buff > 0)
            res = buff;
    }

    return res;
}

int TestHandfullcollisions(float world_step){
    int intersect = 0;
    vec3 dir, pos, lastPos, grav;

    for(int i=0; i < pysics.phyObjectsCount; i++){

        PhysObj *phyObj = &pysics.phyObjects[i];

        pos = lastPos = PhyGetPos(phyObj);

        intersect = TestCollisions(i);

        if(phyObj->physType == PHYSICS_PHYSIC_DINAMIC)
        {

            phyObj->params.moveDir = v3_add(phyObj->params.moveDir, v3_muls(moveDirGrav, worldGravity * (phyObj->params.mass * 0.000001f)));

            pos = v3_add(pos, phyObj->params.moveDir);

            PhySetPos(phyObj, pos);

        }
    }

    return intersect;
}

void InitPhys(){
    CCD_INIT(&ccd);
    ccd.support1 = SphereSupport;
    ccd.support2 = SphereSupport;
    ccd.max_iterations = 200;     // maximal number of iterations
    ccd.epa_tolerance  = 0.01;  // maximal tolerance fro EPA part
}

void WorldSetGraviry(float gravity){
    worldGravity = gravity;
}

float TestIntersectRay(InterRay3DParam *ray, vec3 *q){

    int doit = 1;
    float result = 0, tmax = 0, lastResult = 0;

    vec3 temp = {0, 0, 0};


    for(int i=0; i < pysics.phyObjectsCount; i++){

        InterSphereParam *sphParam = (InterSphereParam *) pysics.phyObjects[i].params.obj;
        InterAABBParam *boxParam = (InterAABBParam *) pysics.phyObjects[i].params.obj;
        InterCapsuleParam *capParam = (InterCapsuleParam *) pysics.phyObjects[i].params.obj;

        if(pysics.phyObjects[i].collType == PHYSICS_COLLIDER_TYPE_SPHERE)
            doit = Intersect3DRaySphere(ray, sphParam->center, &result, &temp);
        else if(pysics.phyObjects[i].collType == PHYSICS_COLLIDER_TYPE_BOX){
            vec3 top_left = {boxParam->size, boxParam->size, boxParam->size};
            doit = Intersect3DRayAABB(ray, v3_sub(boxParam->position, top_left), v3_add(boxParam->position, top_left), &result, &tmax, &temp);
        }
        else if(pysics.phyObjects[i].collType == PHYSICS_COLLIDER_TYPE_CAPSULE){
            vec3 up = {0, capParam->height, 0};
            doit = Intersect3DSegmentCylinder(ray, v3_sub(capParam->position, up), v3_add(capParam->position, up), capParam->radius, &result, &temp);
        }
        else if(pysics.phyObjects[i].collType == PHYSICS_COLLIDER_TYPE_MESH)
            result = Intersect3DRayTriangle(pysics.phyObjects[i].params.obj, ray, &temp);

        if(result > 0 && lastResult == 0 && doit)
        {
            lastResult = result;
            *q = temp;
        }
        else if(result > 0 && result < lastResult && doit)
        {
            lastResult = result;
            *q = temp;
        }

    }

    return lastResult;
}

void SetMoveDirection(int id, float x, float y, float z)
{
    pysics.phyObjects[id].params.moveDir.x = x;
    pysics.phyObjects[id].params.moveDir.y = y;
    pysics.phyObjects[id].params.moveDir.z = z;
}

void SetMass(int id, float mass)
{
    pysics.phyObjects[id].params.mass = mass;
}

void SetElastic(int id, float elastic)
{
    pysics.phyObjects[id].params.elastic = elastic;
}

void SetForce(float force, int id)
{
    pysics.phyObjects[id].params.force = force;
}

int AddPhyObject(void* obj, char collType, char phyType, bool apply_gravity){

    pysics.phyObjectsCount++;

    int id = pysics.phyObjectsCount - 1;

    pysics.phyObjects[id].collType = collType;
    pysics.phyObjects[id].physType = phyType;
    pysics.phyObjects[id].params.mass = 10.0f;
    pysics.phyObjects[id].params.force = 0;
    pysics.phyObjects[id].params.elastic = 1.0f;
    pysics.phyObjects[id].params.moveDir.x = 0;
    pysics.phyObjects[id].params.moveDir.y = 0;
    pysics.phyObjects[id].params.moveDir.z = 0;
    pysics.phyObjects[id].params.obj = obj;
    pysics.phyObjects[id].applyGravity = apply_gravity;

    return id;
}

void PhyDraw(){

}
