#ifndef INCLUDED_WORLD_HEADER
#define INCLUDED_WORLD_HEADER

#include <stdint.h>

#define WORLD_WIDTH 18
#define WORLD_HEIGHT 18

struct _World {
    uint8_t map[WORLD_HEIGHT][WORLD_WIDTH];
};

void WORLD_Reset(struct _World * world);
void WORLD_Update(struct _World * world);

#endif
