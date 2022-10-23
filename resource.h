#ifndef RESOURCE_H
#define RESOURCE_H

#include "camera.h"

#include "gameObject3D.h"

extern Camera2D camera2D;
extern Camera3D camera3D;

extern bool lock_cursor;
extern bool walk, jump, grounded, leftMouse;
extern bool esc_press;

extern int fpsCounter;

extern float yaw;
extern float pitch;

extern float moveSpeed;

extern float cameraSpeed;
extern float sensitivity;

extern float speed;
extern float tiker;

extern float force;

extern char path[];

#endif // RESOURCE_H
