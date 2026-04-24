#include "render.h"

uint8_t frame[RENDER_FRAME_WIDTH];

void RENDER_DrawFrame() {
    for (uint16_t x = 0; x < 640; x += 1) {
        // Find the x point for the ray in screen space
        float cameraSpaceX = 2 * x / (float)(RENDER_FRAME_WIDTH) - 1;
        
        // Compute the vector for the ray
        struct _Vector ray;
        ray.x = direction.x + camera.x * cameraSpaceX;
        ray.y = direction.y + camera.y * cameraSpaceX;
        
        // Distance for the ray to travel
        struct _Vector deltaDistance;
        deltaDistance.x = fabs(1 / ray.x);
        deltaDistance.y = fabs(1 / ray.y);
        
        // What direction the ray is pointing
        struct _Vector stepDirection;
        
        // The distance to move along the ray for a step to the next tile
        struct _Vector sideDistance;
        
        // The player position on the map
        uint8_t mapPositionX = (uint8_t)(player.x);
        uint8_t mapPositionY = (uint8_t)(player.y);
        
        // TODO no fucking clue
        if (ray.x < 0) {
          stepDirection.x = -1;
          sideDistance.x = (player.x - mapPositionX) * deltaDistance.x;
        } else {
          stepDirection.x = 1;
          sideDistance.x = (mapPositionX + 1.0 - player.x) * deltaDistance.x;
        }
        
        if (ray.y < 0) {
          stepDirection.y = -1;
          sideDistance.y = (player.y - mapPositionY) * deltaDistance.y;
        } else {
          stepDirection.y = 1;
          sideDistance.y = (mapPositionY + 1.0 - player.y) * deltaDistance.y;
        }
        
        // The side of the tile we hit (NE/SW)
        uint8_t side;
        
        // Take step after step untill we end up in a tile which is marked as a wall
        while (1) {
            if (sideDistance.x < sideDistance.y) {
                sideDistance.x += deltaDistance.x;
                mapPositionX += stepDirection.x;
                side = 0;
            } else {
                sideDistance.y += deltaDistance.y;
                mapPositionY += stepDirection.y;
                side = 1;
            }

            if (world[mapPositionY][mapPositionX] > 0) {
                break;
            }
        }
        
        // Depending on the side of the wall we hit we need to compute the distance differently
        float distance = 0;
        if (side == 0) {
            distance = sideDistance.x - deltaDistance.x;
        } else {
            distance = sideDistance.y - deltaDistance.y;
        }
        
        if (distance < 0.75f)
            distance = 0.75f;
        frame[x] = (RENDER_FRAME_HEIGHT * 0.5)/distance;
    }
}
