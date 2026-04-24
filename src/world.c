#include "world.h"

struct _Vector player;
struct _Vector camera;
struct _Vector direction;
float playerAngle;

uint8_t world[WORLD_HEIGHT][WORLD_WIDTH] = {
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    {1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1},
    {1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1},
    {1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1,1,0,0,0,0,1},
    {1,1,0,0,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,0,0,0,0,1},
    {1,1,0,0,1,0,0,0,0,0,0,0,0,0,1,0,1,0,1,1,1,1,1,1},
    {1,1,0,0,1,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,1},
    {1,1,0,0,1,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,1},
    {1,1,0,0,1,0,0,0,0,1,1,1,0,0,1,0,1,0,0,0,0,0,0,1},
    {1,1,0,0,1,0,0,0,0,1,0,1,0,0,1,0,1,0,0,0,0,1,1,1},
    {1,1,0,0,1,0,0,0,0,1,0,1,0,0,1,0,1,0,0,0,0,1,1,1},
    {1,1,0,0,1,1,1,0,0,1,1,1,0,0,1,0,1,0,0,0,0,0,0,1},
    {1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,1,0,1,1,1,1,1,1},
    {1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,1,0,1,0,0,0,0,1},
    {1,0,0,0,0,0,1,1,1,0,1,1,1,1,1,0,1,1,1,0,0,0,0,1},
    {1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
    {1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

uint8_t buttons = 0;

void WORLD_Reset() {
    player.x = 2;
    player.y = 2;
    direction.x = 1;
    direction.y = 0;
    camera.x = 0;
    camera.y = 1;
    playerAngle = 0;
}

void WORLD_UpdatePlayer() {
    struct _Vector newPosition;
    
    newPosition.x = player.x;
    newPosition.y = player.y;

    if (buttons & BTN_W) {
        newPosition.x += direction.x * WORLD_MOVE_SPEED;
        newPosition.y += direction.y * WORLD_MOVE_SPEED;
    }

    if (buttons & BTN_S) {
        newPosition.x -= direction.x * WORLD_MOVE_SPEED;
        newPosition.y -= direction.y * WORLD_MOVE_SPEED;
    }
    
    if (world[(uint8_t)player.y][(uint8_t)newPosition.x] == 0) {
        player.x = newPosition.x;
    }
    
    if (world[(uint8_t)newPosition.y][(uint8_t)player.x] == 0) {
        player.y = newPosition.y;
    }
    
    if (buttons & BTN_A) {
        playerAngle -= WORLD_TURN_SPEED;
    }
    if (buttons & BTN_D) {
        playerAngle += WORLD_TURN_SPEED;
    }
    
    if (buttons & BTN_SPACE) {
    }
    
    direction.x = cos(playerAngle);
    direction.y = sin(playerAngle);
    
    camera.x = -direction.y * 0.66f;
    camera.y =  direction.x * 0.66f;
}
