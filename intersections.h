#ifndef INTERSECTIONS_H
#define INTERSECTIONS_H

#include "stdinclude.h"

#include "e_math.h"

typedef struct{
    vec3 position;
    float height;
    float radius;
} PhyCapsuleParam;

typedef struct{
    vec3 center;
    float radius;
} PhySphereParam;

typedef struct{
    vec3 position;
    float size;
} PhyAABBParam;

typedef struct {
    vec3 center; // OBB center point
    vec3 axes; // Local x-, y-, and z-axes
    vec3 halfwidth; // Positive halfwidth extents of OBB along each axis
} PhyOBBParam;

typedef struct{
    vec3 position;
    vec3 direction;
    float distance;
} PhyRayParam;

vec3 ClosestPtPointTriangle(vec3 p, vec3 p0, vec3 p1, vec3 p2);

int TestSphereShape(void *obj1, void *shape, float *dist, float *depth, vec3 *dir);
int TestSphereToSphere(void *obj1, void *obj2, float *dist, float *depth, vec3 *dir);
int TestSphereAABB(void *obj1, void *obj2, float *dist, float *depth, vec3 *dir);
int TestSphereCapsule(void *obj1, void *obj2, float *dist, float *depth, vec3 *dir);
int TestCapsuleCapsule(void *obj1, void *obj2, float *dist, float *depth, vec3 *dir);
int TestCapsuleShape(void *obj1, void *shape, float *dist, float *depth, vec3 *dir);
int TestAABBAABB(void *obj1, void *obj2, float *dist, float *depth, vec3 *dir);
int TestAABBCapsule(void *obj1, void *obj2, float *dist, float *depth, vec3 *dir);
int TestTriangleAABB(vec3 v0, vec3 v1, vec3 v2, void *obj, float *dist, float *depth, vec3 *dir);


int IntersectRaySphere(PhyRayParam *param, vec3 ObjP, float *t, vec3 *q);
int IntersectRayAABB(PhyRayParam *param,  vec3 obj_min, vec3 obj_max, float *tmin,  float *tmax, vec3 *q);
float IntersectRayTriangle(void* shape, PhyRayParam *param, vec3 *q);
int IntersectSegmentCylinder(PhyRayParam *param, vec3 p, vec3 q, float r, float *t, vec3 *q_res);

#endif // INTERSECTIONS_H
