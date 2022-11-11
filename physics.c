#include "physics.h"

#include "engine.h"
#include "textObject.h"

#include "resource.h"

#include "tools.h"

typedef struct{
    PhysObj* phyObjects;
    int phyObjectsCount;
} PysicsStruct;

struct Sphere
{
    vec3 m_Center;
    float m_Radius;
};

PysicsStruct pysics;

float worldSpeed = 0.001f;

float worldGravity = 1.0f;

vec3 moveDirGrav = {0, -1, 0};

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

    for(int j=0; j < pysics.phyObjectsCount; j++){

        if(iter == j)
            continue;

        if(pysics.phyObjects[iter].collType == PHYSICS_COLLIDER_TYPE_SPHERE)
        {
            switch(pysics.phyObjects[j].collType)
            {
                case PHYSICS_COLLIDER_TYPE_SPHERE:
                    buff = IntersectionSphereToSphere(pysics.phyObjects[iter].params.obj, pysics.phyObjects[j].params.obj, &distance, &depth, &dir);
                    break;                    
                case PHYSICS_COLLIDER_TYPE_BOX:
                    buff = IntersectionSphereAABB(pysics.phyObjects[iter].params.obj, pysics.phyObjects[j].params.obj, &distance, &depth, &dir);
                    break;
                case PHYSICS_COLLIDER_TYPE_MESH:
                    buff = IntersectionSphereShape(pysics.phyObjects[iter].params.obj, pysics.phyObjects[j].params.obj, &distance, &depth, &dir);
                    break;
                case PHYSICS_COLLIDER_TYPE_CAPSULE:
                    buff = IntersectionSphereCapsule(pysics.phyObjects[iter].params.obj, pysics.phyObjects[j].params.obj, &distance, &depth, &dir);
                    break;
                default:
                    break;
            }
        }else if(pysics.phyObjects[iter].collType == PHYSICS_COLLIDER_TYPE_CAPSULE)
        {
            switch(pysics.phyObjects[j].collType)
            {
                case PHYSICS_COLLIDER_TYPE_SPHERE:
                    buff = IntersectionSphereCapsule(pysics.phyObjects[j].params.obj, pysics.phyObjects[iter].params.obj, &distance, &depth, &dir);
                    break;
                case PHYSICS_COLLIDER_TYPE_MESH:
                    buff = IntersectionCapsuleShape(pysics.phyObjects[iter].params.obj, pysics.phyObjects[j].params.obj, &distance, &depth, &dir);
                    break;
                case PHYSICS_COLLIDER_TYPE_CAPSULE:
                    buff = IntersectionCapsuleCapsule(pysics.phyObjects[iter].params.obj, pysics.phyObjects[j].params.obj, &distance, &depth, &dir);
                    break;
                case PHYSICS_COLLIDER_TYPE_BOX:
                    buff = IntersectionAABBCapsule(pysics.phyObjects[iter].params.obj, pysics.phyObjects[j].params.obj, &distance, &depth, &dir);
                    break;
                default:
                    break;
            }
        }else if(pysics.phyObjects[iter].collType == PHYSICS_COLLIDER_TYPE_BOX)
        {
            switch(pysics.phyObjects[j].collType)
            {
                case PHYSICS_COLLIDER_TYPE_SPHERE:
                    buff = IntersectionSphereAABB(pysics.phyObjects[j].params.obj, pysics.phyObjects[iter].params.obj, &distance, &depth, &dir);
                    break;
                case PHYSICS_COLLIDER_TYPE_MESH:
                    //buff = IntersectionSphereShape(pysics.phyObjects[iter].params.obj, pysics.phyObjects[j].params.obj, &distance, &depth, &dir);
                    break;
                case PHYSICS_COLLIDER_TYPE_CAPSULE:
                    buff = IntersectionAABBCapsule(pysics.phyObjects[j].params.obj, pysics.phyObjects[iter].params.obj, &distance, &depth, &dir);
                    break;
                case PHYSICS_COLLIDER_TYPE_BOX:
                    buff = IntersectionAABBAABB(pysics.phyObjects[iter].params.obj, pysics.phyObjects[j].params.obj, &distance, &depth, &dir);
                    break;
                default:
                    break;
            }
        }



        if(pysics.phyObjects[iter].physType == PHYSICS_PHYSIC_DINAMIC && buff)
        {
            if(depth < 0.2f)
                depth = 0.2f;

            vec3 dd = v3_muls(dir, depth); //* pysics.phyObjects[iter].params.force);

            pysics.phyObjects[iter].params.moveDir = v3_add(pysics.phyObjects[iter].params.moveDir, dd);

            float counter_force = 0.01f, min_force = 0.1f;

            if(pysics.phyObjects[iter].params.moveDir.x > min_force)
                pysics.phyObjects[iter].params.moveDir.x -= counter_force;


            if(pysics.phyObjects[iter].params.moveDir.x < -min_force)
                pysics.phyObjects[iter].params.moveDir.x += counter_force;


            if(pysics.phyObjects[iter].params.moveDir.z >min_force)
                pysics.phyObjects[iter].params.moveDir.z -= counter_force;

            if(pysics.phyObjects[iter].params.moveDir.z < -min_force)
                pysics.phyObjects[iter].params.moveDir.z += counter_force;

        }

        if(buff > 0)
            res = buff;

    }

    return res;
}

int TestHandfullcollisions(){
    int intersect = 0;
    vec3 dir, pos, lastPos, grav;



    for(int i=0; i < pysics.phyObjectsCount; i++){

        pos = lastPos = PhyGetPos(&pysics.phyObjects[i]);

        intersect = TestCollisions(i);

        if(pysics.phyObjects[i].physType == PHYSICS_PHYSIC_DINAMIC)
        {

            if(!intersect && pysics.phyObjects[i].applyGravity){
                grav = v3_muls(moveDirGrav, worldGravity * pysics.phyObjects[i].params.mass);

                pysics.phyObjects[i].params.moveDir = v3_add(pysics.phyObjects[i].params.moveDir, grav);

                if(pysics.phyObjects[i].params.moveDir.y > 0){
                    pysics.phyObjects[i].params.moveDir.y -= worldGravity * pysics.phyObjects[i].params.mass * 20;

                    if(pysics.phyObjects[i].params.moveDir.y < 1.0f)
                        pysics.phyObjects[i].params.moveDir.y = 0;
                }
            }else if(intersect){
                pos = v3_add(pos, pysics.phyObjects[i].params.moveDir);
            }

            if(pysics.phyObjects[i].applyGravity)
            {
                float minForce = 0.001f;

                if(pysics.phyObjects[i].params.moveDir.y < minForce &&
                   pysics.phyObjects[i].params.moveDir.y > -minForce)
                    pysics.phyObjects[i].params.moveDir.y =0;



                pos = v3_add(pos, pysics.phyObjects[i].params.moveDir);
            }

            if(v3_distance(pos, lastPos) > 10){
                pos = lastPos;
                pysics.phyObjects[i].params.moveDir = (vec3){0, 0, 0};
            }

            PhySetPos(&pysics.phyObjects[i], pos);

        }
    }

    return intersect;
}

void InitPhys(){

    worldGravity *= worldSpeed;

    pysics.phyObjects = (PhysObj *) calloc(0, sizeof(PhysObj));

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

void AddPhyObject(void* obj, char collType, char phyType, bool apply_gravity){

    pysics.phyObjectsCount++;

    pysics.phyObjects = (PhysObj *) realloc(pysics.phyObjects, pysics.phyObjectsCount * sizeof(PhysObj));

    pysics.phyObjects[pysics.phyObjectsCount - 1].collType = collType;
    pysics.phyObjects[pysics.phyObjectsCount - 1].physType = phyType;
    pysics.phyObjects[pysics.phyObjectsCount - 1].params.mass = 1.0f;
    pysics.phyObjects[pysics.phyObjectsCount - 1].params.force = 0;
    pysics.phyObjects[pysics.phyObjectsCount - 1].params.moveDir = (vec3){ 0, 0, 0};
    pysics.phyObjects[pysics.phyObjectsCount - 1].params.obj = obj;
    pysics.phyObjects[pysics.phyObjectsCount - 1].applyGravity = apply_gravity;
}

void PhyDraw(){

}

void PhyDestroy(){

    free(pysics.phyObjects);

}


