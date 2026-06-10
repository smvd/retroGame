#include "render.h"

#include <stdio.h>

float RENDER_CastRay(struct _Vector source, struct _Vector ray, struct _Map * map, float maxDistance) {
    // The distance the ray has to move to go from one x/y side to the next
    struct _Vector stepSize;
    stepSize.x = fabs(1 / ray.x);
    stepSize.y = fabs(1 / ray.y);
    
    // Round the players position to the nearest map tile
    uint8_t mapPositionX = (uint8_t)(source.x);
    uint8_t mapPositionY = (uint8_t)(source.y);
    
    // What direction the rays last step was in
    // -1 or 1 for each direction
    struct _Vector stepDirection;
    
    // The distance that has been moved along the ray
    struct _Vector sideDistance;

    // Compute the rays initial step to align it with a grid square
    if (ray.x < 0) {
        stepDirection.x = -1;
        sideDistance.x = (source.x - mapPositionX) * stepSize.x;
    } else {
        stepDirection.x = 1;
        sideDistance.x = (mapPositionX + 1.0 - source.x) * stepSize.x;
    }
    
    // Same as above but for the y component of the ray
    if (ray.y < 0) {
        stepDirection.y = -1;
        sideDistance.y = (source.y - mapPositionY) * stepSize.y;
    } else {
        stepDirection.y = 1;
        sideDistance.y = (mapPositionY + 1.0 - source.y) * stepSize.y;
    }
    
    // The side of the grid square the ray has hit
    uint8_t side;
    float lastDistance = 0.0f;
        
    // Step along the ray until the grid square is marked as a wall
    while (1) {
        if (sideDistance.x < sideDistance.y) {
            lastDistance = sideDistance.x;
            sideDistance.x += stepSize.x;
            mapPositionX += stepDirection.x;
            side = 0;
        } else {
            lastDistance = sideDistance.y;
            sideDistance.y += stepSize.y;
            mapPositionY += stepDirection.y;
            side = 1;
        }

        if (lastDistance > maxDistance) {
            return maxDistance;
        }

        if (GET_BIT(map->tiles[mapPositionY], mapPositionX) != 0) {
            break;
        }
    }
    
    // Take a step back along the ray to get the exact length
    float distance = 0;
    if (side == 0) {
        distance = sideDistance.x - stepSize.x;
    } else {
        distance = sideDistance.y - stepSize.y;
    }
    
    return distance;
}

void RENDER_DrawWalls(struct _Player * player, struct _Map * map, struct _Frame * frame) {
    struct _Vector camera;
    camera.x = -player->direction.y;
    camera.y =  player->direction.x;
        
    for (uint16_t x = 0; x < RENDER_FRAME_WIDTH; x += 1) {
        // range -1 to 1
        float cameraPlaneFactor = 2 * x / (float)(RENDER_FRAME_WIDTH) - 1;
        
        // Vector from the player to the camera plane
        struct _Vector ray;
        ray.x = player->direction.x + camera.x * cameraPlaneFactor;
        ray.y = player->direction.y + camera.y * cameraPlaneFactor;
            
        float distance = RENDER_CastRay(player->position, ray, map, 255);
        
        if (distance < 0.75f) {
            distance = 0.75f;
        }

        frame->wallBuffer[x] = (RENDER_FRAME_HEIGHT * 0.5)/distance;
    }
}

float RENDER_GetDistance(struct _Vector a, struct _Vector b) {
    return sqrtf(powf(a.x - b.x, 2) + powf(a.y - b.y, 2));
}

uint16_t RENDER_GetScreenSpacePosition(struct _Player * player, struct _Vector camera, struct _Vector object) {
    struct _Vector relative;
    relative.x = object.x - player->position.x;
    relative.y = object.y - player->position.y;
    
    float invDet = 1.0f / (camera.x * player->direction.y - player->direction.x * camera.y);
    
    struct _Vector transform;
    transform.x = invDet * (player->direction.y * relative.x - player->direction.x * relative.y);
    transform.y = invDet * (-camera.y * relative.x + camera.x * relative.y);
    
    return (uint16_t)((RENDER_FRAME_WIDTH / 2) * (1 + transform.x / transform.y));
}

void RENDER_DrawHud(struct _Player * player, struct _Frame * frame, uint8_t rounds[MAP_COUNT]) {
    if (player->shooting) {
        frame->sprites[frame->spriteCount].x = RENDER_FRAME_WIDTH/2-RENDER_SPRITE_SIZE*1.5;
        frame->sprites[frame->spriteCount].y = RENDER_FRAME_HEIGHT-RENDER_SPRITE_SIZE*2.5;
        frame->sprites[frame->spriteCount].size = 3;
        frame->sprites[frame->spriteCount].index = RENDER_FLASH_SPRITE_INDEX;
        frame->sprites[frame->spriteCount].centered = 0;
        frame->spriteCount += 1;
    }
    
    frame->sprites[frame->spriteCount].x = RENDER_FRAME_WIDTH/2-RENDER_SPRITE_SIZE/2;
    frame->sprites[frame->spriteCount].y = RENDER_FRAME_HEIGHT-RENDER_SPRITE_SIZE;
    frame->sprites[frame->spriteCount].size = 1;
    frame->sprites[frame->spriteCount].index = RENDER_GUN_SPRITE_INDEX;
    frame->sprites[frame->spriteCount].centered = 0;
    frame->spriteCount += 1;
    
    for (uint8_t i = 0; i < player->health; i += 1) {
        frame->sprites[frame->spriteCount].x = RENDER_HEART_SPRITE_OFFSET + i * RENDER_HEART_SPRITE_SIZE;
        frame->sprites[frame->spriteCount].y = RENDER_HEART_SPRITE_OFFSET;
        frame->sprites[frame->spriteCount].size = 1;
        frame->sprites[frame->spriteCount].index = RENDER_HEART_SPRITE_INDEX;
        frame->sprites[frame->spriteCount].centered = 0;
        frame->spriteCount += 1;
    }
        
    if (player->hit) {
        frame->sprites[frame->spriteCount].x = RENDER_FRAME_WIDTH/2;
        frame->sprites[frame->spriteCount].y = RENDER_FRAME_HEIGHT/2;
        frame->sprites[frame->spriteCount].size = 4;
        frame->sprites[frame->spriteCount].index = RENDER_BLOOD_SPRITE_INDEX;
        frame->sprites[frame->spriteCount].centered = 1;
        frame->spriteCount += 1;
    }
    
    for (uint8_t i = 0; i < MAP_COUNT; i += 1) {
        frame->sprites[frame->spriteCount].x = RENDER_FRAME_WIDTH - RENDER_SPRITE_SIZE * (MAP_COUNT - i);
        frame->sprites[frame->spriteCount].y = 16;
        frame->sprites[frame->spriteCount].size = 1;
        if (rounds[i] == 0) {frame->sprites[frame->spriteCount].index = RENDER_NULL_CAN_SPRITE_INDEX;}
        else if (rounds[i] == 1) {frame->sprites[frame->spriteCount].index = 0;}
        else if (rounds[i] == 2) {frame->sprites[frame->spriteCount].index = 2;}
        frame->sprites[frame->spriteCount].centered = 0;
        frame->spriteCount += 1;
    }
}

void RENDER_DrawEnemy(struct _Player * player, struct _Player * enemy, struct _Frame * frame) {
    struct _Vector camera;
    camera.x = -player->direction.y;
    camera.y =  player->direction.x;

    if (player->enemyVisible) {
        uint16_t enemyPosition = RENDER_GetScreenSpacePosition(player, camera, enemy->position);
        if (enemyPosition < RENDER_FRAME_WIDTH) {
            uint8_t enemyScale = RENDER_MAX_SCALE - floor(log2(player->enemyDistance));
            frame->sprites[frame->spriteCount].x = enemyPosition - (RENDER_SPRITE_SIZE * enemyScale * 0.5);
            frame->sprites[frame->spriteCount].y = RENDER_FRAME_HEIGHT/2 - (RENDER_SPRITE_SIZE * enemyScale * 0.5);
            frame->sprites[frame->spriteCount].size = enemyScale;
            if (enemy->hit) {frame->sprites[frame->spriteCount].index = enemy->spriteIndex + RENDER_PLAYER_SPRITE_HIT_OFFSET;}
            else {frame->sprites[frame->spriteCount].index = enemy->spriteIndex;}
            frame->spriteCount += 1;
        }
    }
}

void RENDER_DrawMenu(struct _Player * player, struct _Map * map, struct _Frame * frame) {
    frame->spriteCount = 0;
    RENDER_DrawWalls(player, map, frame);

    for (int16_t i = 0; i < RENDER_TITLE_SPRITE_COUNT; i += 1) {
        frame->sprites[frame->spriteCount] = (struct _Sprite){i*RENDER_SPRITE_SIZE, RENDER_SPRITE_SIZE, RENDER_TITLE_SPRITE_INDEX + i, 1, 0};
        frame->spriteCount += 1;
    }
    
    uint16_t offset = (RENDER_FRAME_WIDTH - RENDER_BUTTON_SPRITE_COUNT * RENDER_SPRITE_SIZE) / 2;
    for (int16_t i = 0; i < RENDER_BUTTON_SPRITE_COUNT; i += 1) {
        uint8_t index = i + RENDER_BUTTON_SPRITE_INDEX;
        if (player->ready) {
            index += RENDER_BUTTON_SPRITE_COUNT;
        }
    
        frame->sprites[frame->spriteCount] = (struct _Sprite){i*RENDER_SPRITE_SIZE+offset, RENDER_FRAME_HEIGHT - 128, index, 1, 0};
        frame->spriteCount += 1;
    }
}

void RENDER_DrawFrame(struct _Player * player, struct _Player * enemy, struct _Map * map, struct _Frame * frame, uint8_t rounds[MAP_COUNT]) {
    frame->spriteCount = 0;
    RENDER_DrawWalls(player, map, frame);
    RENDER_DrawEnemy(player, enemy, frame);
    RENDER_DrawHud(player, frame, rounds);
}
