#ifndef INCLUDED_RENDER_HEADER
#define INCLUDED_RENDER_HEADER

#include <stdint.h>
#include <stdlib.h>

#include "vector.h"
#include "world.h"

#define RENDER_FRAME_WIDTH 640
#define RENDER_FRAME_HEIGHT 360

extern uint8_t frame[RENDER_FRAME_WIDTH];

void RENDER_Reset();
void RENDER_DrawFrame();

#endif
