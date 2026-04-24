#include <stdio.h>

#include "render.h"
#include "world.h"
#include "player.h"

// #define ON_FPGA
#ifdef ON_FPGA
void MAIN_InitVideo() {}
void MAIN_CloseVideo() {}
void MAIN_UpdateFrame() {}
void MAIN_PollInput() {}
#else
#include <raylib.h>

uint8_t activePlayer = 0;

void MAIN_InitVideo() {
    InitWindow(RENDER_FRAME_WIDTH*4, RENDER_FRAME_HEIGHT*2, "retro shiz");
}

void MAIN_CloseVideo() {
    CloseWindow();
}

void MAIN_UpdateFrame(struct _Frame * frameA, struct _Frame * frameB) {
    BeginDrawing();
    ClearBackground(BLACK);
    
    for (uint16_t x = 0; x < RENDER_FRAME_WIDTH; x += 1) {
        DrawRectangle(x*2, RENDER_FRAME_HEIGHT / 2 - frameA->wallBuffer[x], 2, frameA->wallBuffer[x]*4, (Color){frameA->wallBuffer[x], frameA->wallBuffer[x], frameA->wallBuffer[x], 255});
        DrawRectangle(x*2 + RENDER_FRAME_WIDTH*2, RENDER_FRAME_HEIGHT / 2 - frameB->wallBuffer[x], 2, frameB->wallBuffer[x]*4, (Color){frameB->wallBuffer[x], frameB->wallBuffer[x], frameB->wallBuffer[x], 255});
    }

    EndDrawing();
}
void MAIN_PollInput(struct _Player * playerA, struct _Player * playerB) {
    if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT)) {
        activePlayer = !activePlayer;
    }
    
    playerA->shoot = 0; 
    playerB->shoot = 0; 

    struct _Player * player = playerA;
    if (activePlayer) {
        player = playerB;
    }
    
    if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) {
        player->shoot = 1;
    }
    
    float leftStickX = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
    float leftStickY = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);
    float rightStickX = GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_X);
    
    
    if (leftStickX > -0.1 && leftStickX < 0.1) leftStickX = 0.0f;
    if (leftStickY > -0.1 && leftStickY < 0.1) leftStickY = 0.0f;
    if (rightStickX > -0.1 && rightStickX < 0.1) rightStickX = 0.0f;
    
    player->xMove = (int8_t)(leftStickX * 127);
    player->yMove = (int8_t)(leftStickY * 127);
    player->turn = (int8_t)(rightStickX * 127);
}
#endif

int main() {    
    MAIN_InitVideo();

    struct _World world;
    WORLD_Reset(&world);
    
    struct _Player playerA;
    PLAYER_Reset(&playerA, (struct _Vector){2, 2}, (struct _Vector){1, 0});
    
    struct _Player playerB;
    PLAYER_Reset(&playerB, (struct _Vector){15, 15}, (struct _Vector){-1, 0});
    
    struct _Frame frameA;
    struct _Frame frameB;
    
    while (1) {
        MAIN_PollInput(&playerA, &playerB);
        
        WORLD_Update(&world);
        PLAYER_Update(&playerA, &world);
        PLAYER_Update(&playerB, &world);
        
        RENDER_DrawFrame(&playerA, &playerB, &world, &frameA);
        RENDER_DrawFrame(&playerB, &playerA, &world, &frameB);

        MAIN_UpdateFrame(&frameA, &frameB);
    }
    
    MAIN_CloseVideo();

    return 0;
}

// main
//      system specific driver code
// render
//      uses the world to fill out a given frame
// world
//      handles all the world info like the camera, player, map, enemies, movement, etc
