#ifndef INCLUDED_MAP_HEADER

#include <stdint.h>
#include "vector.h"

#define MAP_WIDTH 16
#define MAP_HEIGHT 16
#define MAP_COUNT 1

struct _Map {
    uint16_t tiles[MAP_HEIGHT];
    struct _Vector playerPositionA;
    struct _Vector playerDirectionA;
    struct _Vector playerPositionB;
    struct _Vector playerDirectionB;
};

struct _Map * MAP_SelectRandom();

#endif
