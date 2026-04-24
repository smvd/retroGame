#include "player.h"

void PLAYER_Reset(struct _Player * player, struct _Vector position, struct _Vector direction) {
    player->position = position;
    player->direction = direction;
}

void PLAYER_Update(struct _Player * player, struct _World * world) {
    float fx = player->xMove / 127.0f;
    float fy = player->yMove / 127.0f;
    float tr = player->turn  / 127.0f;
    
    player->direction = VECTOR_Rotate(player->direction, tr * PLAYER_TURN_SPEED);
    
    struct _Vector newPosition = player->position;
    
    newPosition.x -= player->direction.x * fy * PLAYER_MOVE_SPEED;
    newPosition.y -= player->direction.y * fy * PLAYER_MOVE_SPEED;
    
    newPosition.x -= player->direction.y * fx * PLAYER_STRAFE_SPEED;
    newPosition.y += player->direction.x * fx * PLAYER_STRAFE_SPEED;

    if (world->map[(uint8_t)player->position.y][(uint8_t)newPosition.x] == 0) {player->position.x = newPosition.x;} 
    if (world->map[(uint8_t)newPosition.y][(uint8_t)player->position.x] == 0) {player->position.y = newPosition.y;}
}
