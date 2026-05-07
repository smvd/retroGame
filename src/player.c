#include "player.h"

void PLAYER_Reset(struct _Player * player, struct _Vector position, struct _Vector direction) {
    player->position = position;
    player->direction = direction;
    player->shooting = 0;
    player->shootingTimer = 0;
    player->health = PLAYER_HEALTH;
    player->hit = 0;
    player->hitTimer = 0;
    
    player->xMove = 0;
    player->yMove = 0; 
    player->turn = 0;
    player->shoot = 0;
}

void PLAYER_Update(struct _Player * player, struct _Player * enemy, struct _World * world) {
    struct _Vector enemyDirection;
    
    enemyDirection.x = enemy->position.x - player->position.x;
    enemyDirection.y = enemy->position.y - player->position.y;
    
    float len = sqrtf(enemyDirection.x * enemyDirection.x + enemyDirection.y * enemyDirection.y);
    
    enemyDirection.x /= len;
    enemyDirection.y /= len;
    
    float directDistance = RENDER_GetDistance(player->position, enemy->position);
    float obstacleDistance = RENDER_CastRay(player->position, enemyDirection, world, directDistance+1.0f);
    
    player->enemyDistance = directDistance;
    player->enemyVisible = obstacleDistance >= directDistance;

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
    
    if (player->shoot && player->shootingTimer == 0) {
        player->shooting = 1;
        player->shootingTimer = PLAYER_SHOOT_ANIMATION_LENGTH;
        
        float dotProduct = player->direction.x * enemyDirection.x + player->direction.y * enemyDirection.y;
        if (dotProduct > PLAYER_HIT_ANGLE) {
            enemy->hit = 1;
            enemy->hitTimer = PLAYER_HIT_ANIMATION_LENGTH;
            enemy->health -= 1;
        }
    }
    
    if (player->shooting && player->shootingTimer == 0){
        player->shooting = 0;
        player->shootingTimer = PLAYER_SHOOT_COOLDOWN;
    }
    
    if (player->hit && player->hitTimer == 0) {
        player->hit = 0;
    }
    
    if (player->shootingTimer != 0) {player->shootingTimer -= 1;}
    if (player->hitTimer != 0) {player->hitTimer -= 1;}
}
