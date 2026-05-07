#ifndef INCLUDED_RENDER_HEADER
#define INCLUDED_RENDER_HEADER

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define RENDER_FRAME_WIDTH 640
#define RENDER_FRAME_HEIGHT 360

#define RENDER_SPRITE_LIMIT 128

#define RENDER_MAX_SCALE 4

#include "vector.h"
#include "world.h"
#include "player.h"

struct _Player;

struct _Sprite {
    int16_t x;
    int16_t y;
    uint8_t index;
    uint8_t size;
};

struct _Frame {
    uint8_t wallBuffer[RENDER_FRAME_WIDTH];
    struct _Sprite sprites[RENDER_SPRITE_LIMIT];
    uint8_t spriteCount;
};

void RENDER_DrawFrame(struct _Player * player, struct _Player * enemy, struct _World * world, struct _Frame * frame);
uint16_t RENDER_GetScreenSpacePosition(struct _Player * player, struct _Vector camera, struct _Vector object);
float RENDER_CastRay(struct _Vector source, struct _Vector ray, struct _World * world, float maxDistance);
float RENDER_GetDistance(struct _Vector a, struct _Vector b);

#endif
