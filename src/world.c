#include "world.h"

uint8_t world[8][8] = {
    {1,1,1,1,1,1,1,1},
    {1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1},
    {1,0,0,1,1,0,0,1},
    {1,0,0,1,1,0,0,1},
    {1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1},
};

struct _FX_Vector player;
struct _FX_Vector camera;
struct _FX_Vector direction;

uint8_t buttons = 0;

void WORLD_Reset() {
    player.x = TO_FX(3);
    player.y = TO_FX(3);
    direction.x = TO_FX(1);
    direction.y = TO_FX(0);
    camera.x = 0;
    camera.y = (fx32_t)(0.66 * 65536);
}

void WORLD_UpdatePlayer() {
    printf("%d\n", buttons);

    if (buttons & BTN_A) {
        player.x += direction.x >> 4;
        player.y += direction.y >> 4;
    }

    if (buttons & BTN_D) {
        player.x -= direction.x >> 4;
        player.y -= direction.y >> 4;
    }
}
