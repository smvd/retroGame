#include "render.h"

uint8_t frame[RENDER_FRAME_WIDTH];

void RENDER_DrawFrame() {
    for (uint16_t x = 0; x < 640; x += 1) {
        fx32_t cameraX = DIV(TO_FX(2*x), TO_FX(RENDER_FRAME_WIDTH)) - TO_FX(1);
    
        struct _FX_Vector ray;
        
        ray.x = direction.x + MUL(camera.x, cameraX);
        ray.y = direction.y + MUL(camera.y, cameraX);
        
        struct _FX_Vector delta;
        delta.x = (ray.x == 0) ? TO_FX(999999) : ABS(DIV(TO_FX(1), ray.x));
        delta.y = (ray.y == 0) ? TO_FX(999999) : ABS(DIV(TO_FX(1), ray.y));

        struct _Vector map;
        map.x = FROM_FX(player.x);
        map.y = FROM_FX(player.y);

        struct _FX_Vector step;
        struct _FX_Vector distance;
        
        if (ray.x < 0) {
            step.x = -1;
            distance.x = MUL((player.x - TO_FX(map.x)), delta.x);
        } else {
            step.x = 1;
            distance.x = MUL((TO_FX(map.x+1) - player.x), delta.x);
        }
        
        if (ray.y < 0) {
            step.y = -1;
            distance.y = MUL((player.y - TO_FX(map.y)), delta.y);
        } else {
            step.y = 1;
            distance.y = MUL((TO_FX(map.y+1) - player.y), delta.y);
        }
        
        uint8_t side;
        
        while (1) {
            if (distance.x < distance.y) {
                distance.x += delta.x;
                map.x += step.x;
                side = 0;
            } else {
                distance.y += delta.y;
                map.y += step.y;
                side = 1;
            }
        
            if (world[map.y][map.x] != 0) {
                break;
            }
        }
        
        fx32_t trueDistance;
        
        if (side == 0) {
            trueDistance = distance.x - delta.x;
        } else {
            trueDistance = distance.y - delta.y;
        }
        
        frame[x] = 320 / FROM_FX(trueDistance);
    }
}
