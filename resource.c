#include "resource.h"

Camera2D camera2D;
Camera3D camera3D;

bool lock_cursor = true;
bool walk = false, jump = false, grounded = false, leftMouse = false;
bool esc_press = false, left_mouse_press = false;
bool showconsole = false;

vec3 light_direction;

int fpsCounter = 0;

float yaw = 90.f;
float pitch = 0;

float moveSpeed = 2.0f;

float cameraSpeed = 3.5f;
float sensitivity = 2.0f;

float speed = 100;
float tiker = 0;

float force = 0;

char path[] = "/home/ilia/Projects/EngineTest";

RenderTexture render_window;
RenderTexture render_image;
RenderTexture render_shadow[MAX_SOME_SHADOWS];
RenderTexture render_cube_shadow[MAX_CUBE_SHADOWS];
RenderTexture render_spot_shadow[MAX_SPOT_SHADOWS];
