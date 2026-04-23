#include <stdint.h>
#include <raylib.h>

#include "render.h"

// #define ON_FPGA
#ifdef ON_FPGA
void MAIN_InitVideo() {}
void MAIN_CloseVideo() {}
void MAIN_UpdateFrame() {}
void MAIN_PollInput() {}
#else
void MAIN_InitVideo() {
    InitWindow(RENDER_FRAME_WIDTH*2, RENDER_FRAME_HEIGHT*2, "retro shiz");
}

void MAIN_CloseVideo() {
    CloseWindow();
}

void MAIN_UpdateFrame() {
    BeginDrawing();
    ClearBackground(BLACK);
    
    for (uint16_t x = 0; x < RENDER_FRAME_WIDTH; x += 1) {
        DrawRectangle(x*2, (RENDER_FRAME_HEIGHT*0.5 + frame[x]) * 2, 2, 2, WHITE);
        DrawRectangle(x*2, (RENDER_FRAME_HEIGHT*0.5 - frame[x]) * 2, 2, 2, WHITE);
    }
    
    EndDrawing();
}

void MAIN_PollInput() {
    if (IsKeyDown(KEY_W)) {buttons |= BTN_W;} else {buttons &= ~BTN_W;}
    if (IsKeyDown(KEY_A)) {buttons |= BTN_A;} else {buttons &= ~BTN_A;}
    if (IsKeyDown(KEY_S)) {buttons |= BTN_S;} else {buttons &= ~BTN_S;}
    if (IsKeyDown(KEY_D)) {buttons |= BTN_D;} else {buttons &= ~BTN_D;}
    if (IsKeyDown(KEY_SPACE)) {buttons |= BTN_SPACE;}
}
#endif

int main() {
    MAIN_InitVideo();
    
    WORLD_Reset();
    
    while (1) {
        MAIN_PollInput();
        
        WORLD_UpdatePlayer();
        
        RENDER_DrawFrame();

        MAIN_UpdateFrame();
    }
    
    MAIN_CloseVideo();

    return 0;
}
