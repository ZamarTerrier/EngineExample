#ifndef PAINTER_H
#define PAINTER_H

#include <engine.h>

void PainterMouseKeyCallback(void* window, int button, int action, int mods);

void PainterInit();
void PainterUpdate();
void PainterDraw();
void PainterDestroy();

#endif // PAINTER_H
