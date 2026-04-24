#ifndef INCLUDED_RENDER_HEADER
#define INCLUDED_RENDER_HEADER

#include <stdint.h>
#include <stdlib.h>

#define RENDER_FRAME_WIDTH 640
#define RENDER_FRAME_HEIGHT 360

#define RENDER_SPRITE_LIMIT 128

#include "vector.h"
#include "world.h"
#include "player.h"

struct _Player;

struct _Sprite {
    uint16_t x;
    uint16_t y;
    uint8_t index;
    uint8_t size;
};

struct _Frame {
    uint8_t wallBuffer[RENDER_FRAME_WIDTH];
    struct _Sprite sprites[RENDER_SPRITE_LIMIT];
    uint8_t spriteCount;
};

void RENDER_DrawFrame(struct _Player * player, struct _Player * enemy, struct _World * world, struct _Frame * frame);

#endif
