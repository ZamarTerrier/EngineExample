#ifndef GAME_H
#define GAME_H

#include "engine.h"

extern int fpsCounter;

void Init();

void Update(double deltaTime);

void Draw();

void CleanUp();

#endif // GAME_H
