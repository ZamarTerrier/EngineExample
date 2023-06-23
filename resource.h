#ifndef RESOURCE_H
#define RESOURCE_H

#include "e_camera.h"

#include "gameObject3D.h"
#include "render_texture.h"

#define MAX_SOME_SHADOWS 4
#define MAX_CUBE_SHADOWS 2
#define MAX_SPOT_SHADOWS 2

#define TEXTURE_MAP_SIZE 4096

extern Camera2D camera2D;
extern Camera3D camera3D;

extern vec3 light_direction;

extern bool lock_cursor;
extern bool walk, jump, grounded, leftMouse;
extern bool esc_press, left_mouse_press;
extern bool showconsole;

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

extern RenderTexture render_window;
extern RenderTexture render_image;
extern RenderTexture render_shadow[MAX_SOME_SHADOWS];
extern RenderTexture render_cube_shadow[MAX_CUBE_SHADOWS];
extern RenderTexture render_spot_shadow[MAX_SPOT_SHADOWS];
extern RenderTexture render_geometry;

#endif // RESOURCE_H
