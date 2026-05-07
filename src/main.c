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

Texture2D textures[32];
RenderTexture2D screenA;
RenderTexture2D screenB;

uint8_t activePlayer = 0;

void MAIN_InitVideo() {
    InitWindow(RENDER_FRAME_WIDTH*4, RENDER_FRAME_HEIGHT*2, "retro shiz");
    
    Image image = LoadImage("gun.png");
    textures[0] = LoadTextureFromImage(image);
    UnloadImage(image);

    image = LoadImage("flash.png");
    textures[1] = LoadTextureFromImage(image);
    UnloadImage(image);
        
    image = LoadImage("enemy.png");
    textures[2] = LoadTextureFromImage(image);
    UnloadImage(image);
    
    image = LoadImage("enemyHit.png");
    textures[3] = LoadTextureFromImage(image);
    UnloadImage(image);
    
    image = LoadImage("health.png");
    textures[4] = LoadTextureFromImage(image);
    UnloadImage(image);
    
    screenA = LoadRenderTexture(640, 360);
    screenB = LoadRenderTexture(640, 360);
}

void MAIN_CloseVideo() {
    CloseWindow();
}

void MAIN_UpdateFrame(struct _Frame * frameA, struct _Frame * frameB) {
    BeginTextureMode(screenA);
    ClearBackground(BLACK);
    
    for (uint16_t x = 0; x < RENDER_FRAME_WIDTH; x += 1) {
        DrawRectangle(x, RENDER_FRAME_HEIGHT / 2 - frameA->wallBuffer[x], 1, frameA->wallBuffer[x] * 2, (Color){frameA->wallBuffer[x], frameA->wallBuffer[x], frameA->wallBuffer[x], 255});
    }
    
    for (uint8_t i = 0; i < frameA->spriteCount; i += 1) {
        uint16_t spriteCenterOffset = (64 * frameA->sprites[i].size) / 2;
        DrawTextureEx(textures[frameA->sprites[i].index], (Vector2){frameA->sprites[i].x - spriteCenterOffset, frameA->sprites[i].y - spriteCenterOffset},  0.0f, (float)frameA->sprites[i].size, WHITE);
    }
    EndTextureMode();
    
    BeginTextureMode(screenB);
    ClearBackground(BLACK);
    
    for (uint16_t x = 0; x < RENDER_FRAME_WIDTH; x += 1) {
        DrawRectangle(x, RENDER_FRAME_HEIGHT / 2 - frameB->wallBuffer[x], 1, frameB->wallBuffer[x] * 2, (Color){frameB->wallBuffer[x], frameB->wallBuffer[x], frameB->wallBuffer[x], 255});
    }
    
    for (uint8_t i = 0; i < frameB->spriteCount; i += 1) {
        uint16_t spriteCenterOffset = (64 * frameB->sprites[i].size) / 2;
        DrawTextureEx(textures[frameB->sprites[i].index], (Vector2){frameB->sprites[i].x - spriteCenterOffset, frameB->sprites[i].y - spriteCenterOffset},  0.0f, (float)frameB->sprites[i].size, WHITE);
    }
    EndTextureMode();

    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexturePro(
        screenA.texture,
        (Rectangle){ 0, 0, 640, -360 },   // flipped Y
        (Rectangle){ 0, 0, RENDER_FRAME_WIDTH * 2, RENDER_FRAME_HEIGHT * 2},
        (Vector2){ 0, 0 },
        0.0f,
        WHITE
    );
    DrawTexturePro(
        screenB.texture,
        (Rectangle){0, 0, 640, -360 },   // flipped Y
        (Rectangle){RENDER_FRAME_WIDTH * 2, 0, RENDER_FRAME_WIDTH * 2, RENDER_FRAME_HEIGHT * 2},
        (Vector2){0, 0},
        0.0f,
        WHITE
    );
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
        PLAYER_Update(&playerA, &playerB, &world);
        PLAYER_Update(&playerB, &playerA, &world);
        
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
