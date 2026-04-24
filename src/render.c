#include "render.h"

void RENDER_DrawFrame(struct _Player * player, struct _Player * enemy, struct _World * world, struct _Frame * frame) {
    for (uint16_t x = 0; x < RENDER_FRAME_WIDTH; x += 1) {
        // Where along the camera plane the ray must intersect
        // range -1 to 1
        float cameraPlaneFactor = 2 * x / (float)(RENDER_FRAME_WIDTH) - 1;
        
        // Derive the camera plane from the players direction
        struct _Vector camera;
        camera.x = -player->direction.y;
        camera.y =  player->direction.x;
        
        // Vector from the player to the camera plane
        struct _Vector ray;
        ray.x = player->direction.x + camera.x * cameraPlaneFactor;
        ray.y = player->direction.y + camera.y * cameraPlaneFactor;
        
        // The distance the ray has to move to go from one x/y side to the next
        struct _Vector stepSize;
        stepSize.x = fabs(1 / ray.x);
        stepSize.y = fabs(1 / ray.y);
        
        // Round the players position to the nearest map tile
        uint8_t mapPositionX = (uint8_t)(player->position.x);
        uint8_t mapPositionY = (uint8_t)(player->position.y);
        
        // What direction the rays last step was in
        // -1 or 1 for each direction
        struct _Vector stepDirection;
        
        // The distance that has been moved along the ray
        struct _Vector sideDistance;

        // Compute the rays initial step to align it with a grid square
        if (ray.x < 0) {
            stepDirection.x = -1;
            sideDistance.x = (player->position.x - mapPositionX) * stepSize.x;
        } else {
            stepDirection.x = 1;
            sideDistance.x = (mapPositionX + 1.0 - player->position.x) * stepSize.x;
        }
        
        // Same as above but for the y component of the ray
        if (ray.y < 0) {
            stepDirection.y = -1;
            sideDistance.y = (player->position.y - mapPositionY) * stepSize.y;
        } else {
            stepDirection.y = 1;
            sideDistance.y = (mapPositionY + 1.0 - player->position.y) * stepSize.y;
        }
        
        // The side of the grid square the ray has hit
        uint8_t side;
        
        // Step along the ray until the grid square is marked as a wall
        while (1) {
            if (sideDistance.x < sideDistance.y) {
                sideDistance.x += stepSize.x;
                mapPositionX += stepDirection.x;
                side = 0;
            } else {
                sideDistance.y += stepSize.y;
                mapPositionY += stepDirection.y;
                side = 1;
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
        
        if (distance < 0.75f) {
            distance = 0.75f;
        }
        frame->wallBuffer[x] = (RENDER_FRAME_HEIGHT * 0.5)/distance;
    }
    
    
}
