#ifndef INCLUDED_PLAYER_HEADER
#define INCLUDED_PLAYER_HEADER

#define INPUT_UP            (0x01 << 0)
#define INPUT_DOWN          (0x01 << 1)
#define INPUT_LEFT          (0x01 << 2)
#define INPUT_RIGHT         (0x01 << 3)
#define INPUT_TURN_LEFT     (0x01 << 4)
#define INPUT_TURN_RIGHT    (0x01 << 5)
#define INPUT_SHOOT         (0x01 << 6)

#define PLAYER_TURN_SPEED 0.0025
#define PLAYER_MOVE_SPEED 0.005
#define PLAYER_STRAFE_SPEED 0.005

#define PLAYER_SHOOT_ANIMATION_LENGTH 200
#define PLAYER_SHOOT_COOLDOWN 1000
#define PLAYER_HIT_ANIMATION_LENGTH 400

#define PLAYER_HIT_ANGLE 0.99
#define PLAYER_HEALTH 3

#include "vector.h"
#include "render.h"
#include "world.h"

struct _Player {
    uint8_t wallBuffer[RENDER_FRAME_WIDTH];
    struct _Vector position;
    struct _Vector direction;
    
    int8_t xMove;
    int8_t yMove;
    int8_t turn;
    uint8_t shoot;
    
    uint8_t shooting;
    uint16_t shootingTimer;
    
    uint8_t health;
    uint8_t hit;
    uint16_t hitTimer;
    
    float enemyDistance;
    uint8_t enemyVisible;
};

void PLAYER_Reset(struct _Player * player, struct _Vector position, struct _Vector direction);
void PLAYER_Update(struct _Player * player, struct _Player * enemy, struct _World * world);

#endif
