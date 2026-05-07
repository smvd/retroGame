#include "render.h"

void RENDER_DrawFrame(struct _Player * player, struct _Player * enemy, struct _World * world, struct _Frame * frame) {
    // Derive the camera plane from the players direction
    struct _Vector camera;
    camera.x = -player->direction.y;
    camera.y =  player->direction.x;
        
    for (uint16_t x = 0; x < RENDER_FRAME_WIDTH; x += 1) {
        // Where along the camera plane the ray must intersect
        // range -1 to 1
        float cameraPlaneFactor = 2 * x / (float)(RENDER_FRAME_WIDTH) - 1;
        
        // Vector from the player to the camera plane
        struct _Vector ray;
        ray.x = player->direction.x + camera.x * cameraPlaneFactor;
        ray.y = player->direction.y + camera.y * cameraPlaneFactor;
        
        float distance = RENDER_CastRay(player->position, ray, world, 255);
        
        if (distance < 0.75f) {
            distance = 0.75f;
        }

        frame->wallBuffer[x] = (RENDER_FRAME_HEIGHT * 0.5)/distance;
    }
    
    frame->spriteCount = 0;
    
    if (player->enemyVisible) {
        uint16_t enemyPosition = RENDER_GetScreenSpacePosition(player, camera, enemy->position);
        if (enemyPosition < RENDER_FRAME_WIDTH) {
            frame->sprites[frame->spriteCount].x = enemyPosition;
            frame->sprites[frame->spriteCount].y = RENDER_FRAME_HEIGHT/2;
            frame->sprites[frame->spriteCount].size = RENDER_MAX_SCALE - floor(log2(player->enemyDistance));
            if (enemy->hit) {frame->sprites[frame->spriteCount].index = 3;}
            else {frame->sprites[frame->spriteCount].index = 2;}
            frame->spriteCount += 1;
        }
    }
    
    if (player->shooting) {
        frame->sprites[frame->spriteCount].x = RENDER_FRAME_WIDTH/2;
        frame->sprites[frame->spriteCount].y = RENDER_FRAME_HEIGHT - 64;
        frame->sprites[frame->spriteCount].size = 2;
        frame->sprites[frame->spriteCount].index = 1;
        frame->spriteCount += 1;
    }
    
    frame->sprites[frame->spriteCount].x = RENDER_FRAME_WIDTH/2;
    frame->sprites[frame->spriteCount].y = RENDER_FRAME_HEIGHT - 32;
    frame->sprites[frame->spriteCount].size = 1;
    frame->sprites[frame->spriteCount].index = 0;
    frame->spriteCount += 1;
    
    // sprites get recentered for a 64x64 so for a smaller sprite i need to revert the offset
    for (uint8_t i = 0; i < player->health; i += 1) {
        frame->sprites[frame->spriteCount].x = 34 + i * 18;
        frame->sprites[frame->spriteCount].y = 34;
        frame->sprites[frame->spriteCount].size = 1;
        frame->sprites[frame->spriteCount].index = 4;
        frame->spriteCount += 1;
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

float RENDER_CastRay(struct _Vector source, struct _Vector ray, struct _World * world, float maxDistance) {
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

        if (world->map[mapPositionY][mapPositionX] > 0) {
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
