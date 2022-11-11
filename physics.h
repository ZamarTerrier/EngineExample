#ifndef PHYSICS_H
#define PHYSICS_H

#include "gameObject3D.h"

#include "engine_includes.h"

#include "rayIntersections3D.h"
#include "intersections3D.h"

enum PhysicsObjectType{
    PHYSICS_COLLIDER_TYPE_SPHERE,
    PHYSICS_COLLIDER_TYPE_BOX,
    PHYSICS_COLLIDER_TYPE_CAPSULE,
    PHYSICS_COLLIDER_TYPE_MESH,
    PHYSICS_COLLIDER_TYPE_TERRAIN
};

enum PhysicType{
    PHYSICS_PHYSIC_STATIC,
    PHYSICS_PHYSIC_DINAMIC
};

typedef struct{
    void* obj;
    vec3 moveDir;
    float mass;
    float force;
} PhyParam;

typedef struct{
    char collType;
    char physType;
    bool applyGravity;
    PhyParam params;
} PhysObj;

void InitPhys();

float TestIntersectRay(InterRay3DParam *ray, vec3 *q);

int TestHandfullcollisions();
void WorldStep();

void AddPhyObject(void * go, char collType, char phyType, bool apply_gravity);

void PhyDraw();
void PhyDestroy();
#endif // PHYSICS_H
