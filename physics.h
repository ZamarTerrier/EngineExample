#ifndef PHYSICS_H
#define PHYSICS_H

#include "gameObject3D.h"

#include "engine_includes.h"

#include "rayIntersections3D.h"
#include "intersections3D.h"

typedef enum{
    PHYSICS_COLLIDER_TYPE_PLANE,
    PHYSICS_COLLIDER_TYPE_SPHERE,
    PHYSICS_COLLIDER_TYPE_BOX,
    PHYSICS_COLLIDER_TYPE_CAPSULE,
    PHYSICS_COLLIDER_TYPE_MESH,
    PHYSICS_COLLIDER_TYPE_TERRAIN
} PhysicsObjectType;

typedef enum{
    PHYSICS_PHYSIC_STATIC,
    PHYSICS_PHYSIC_DINAMIC
} PhysicType;

typedef struct{
    void* obj;
    vec3 moveDir;
    float mass;
    float force;
    float elastic;
    bool inside;
} PhyParam;

typedef struct{
    PhysicsObjectType collType;
    PhysicType physType;
    bool applyGravity;
    PhyParam params;
} PhysObj;

void InitPhys();

float TestIntersectRay(InterRay3DParam *ray, vec3 *q);

int TestHandfullcollisions(float world_step);
void WorldStep();

void SetMass(int id, float mass);
void SetElastic(int id, float elastic);
void SetForce(float force, int id);

void SetMoveDirection(int id, float x, float y, float z);

int AddPhyObject(void* obj, char collType, char phyType, bool apply_gravity);

void PhyDraw();
void PhyDestroy();
#endif // PHYSICS_H
