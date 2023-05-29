#ifndef GAME_SYS_H
#define GAME_SYS_H

#include "engine.h"

void SysMouseKeyCallback(void* window, int button, int action, int mods);

void SysInit();
void SysUpdate(double deltaTime);

void SysCalcDirLight();

#endif // GAME_SYS_H
