#ifndef INCLUDED_RENDER_HEADER
#define INCLUDED_RENDER_HEADER

#include "player.h"
#include "map.h"
#include "bitwise.h"

#define RENDER_FRAME_WIDTH 640
#define RENDER_FRAME_HEIGHT 360

#define RENDER_SPRITE_SIZE 64
#define RENDER_SPRITE_LIMIT 128

#define RENDER_TITLE_SPRITE_COUNT 10
#define RENDER_BUTTON_SPRITE_COUNT 3

#define RENDER_GUN_SPRITE_INDEX 4
#define RENDER_FLASH_SPRITE_INDEX 5
#define RENDER_HEART_SPRITE_INDEX 6
#define RENDER_TITLE_SPRITE_INDEX 7
#define RENDER_BUTTON_SPRITE_INDEX 17
#define RENDER_BLOOD_SPRITE_INDEX 23

#define RENDER_NULL_CAN_SPRITE_INDEX 24

#define RENDER_HEART_SPRITE_SIZE 16
#define RENDER_HEART_SPRITE_OFFSET 10

#define RENDER_MAX_SCALE 4
#define RENDER_FLASH_OFFSET RENDER_SPRITE_SIZE*1.5
#define RENDER_PLAYER_SPRITE_HIT_OFFSET 1

struct _Sprite {
    int16_t x;
    int16_t y;
    uint8_t index;
    uint8_t size;
    uint8_t centered;
};

struct _Frame {
    uint8_t wallBuffer[RENDER_FRAME_WIDTH];
    struct _Sprite sprites[RENDER_SPRITE_LIMIT];
    uint8_t spriteCount;
};

void RENDER_DrawMenu(struct _Player * player, struct _Map * map, struct _Frame * frame);
void RENDER_DrawFrame(struct _Player * player, struct _Player * enemy, struct _Map * map, struct _Frame * frame, uint8_t rounds[MAP_COUNT]);
float RENDER_GetDistance(struct _Vector a, struct _Vector b);
float RENDER_CastRay(struct _Vector source, struct _Vector ray, struct _Map * map, float maxDistance);

#endif
