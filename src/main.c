#include <stdio.h>
#include <raylib.h>

#include "render.h"
#include "map.h"
#include "player.h"

/*
    0, player 1 normal
    1, player 1 hit
    2, player 2 normal
    3, player 2 hit
    4, gun
    5, muzzleflash
    6, health
    7, title 1
    8, title 2
    9, title 3
    10, title 4
    11, title 5
    12, title 6
    13, title 7
    14, title 8
    15, title 9
    16, title 10
    17, button 1
    18, button 2
    19, button 3
    20, button active 1
    21, button active 2
    22, button active 3
*/

enum _state {
    STATE_MAIN_MENU = 0x01,
    STATE_PLAYING = 0x02,
    STATE_END_SCREEN = 0x03
};

Texture2D textures[32];
RenderTexture2D screenA;
RenderTexture2D screenB;

uint8_t activePlayer = 0;

void MAIN_InitVideo() {
    InitWindow(RENDER_FRAME_WIDTH*2, RENDER_FRAME_HEIGHT, "retro shiz");
    
    char buffer[64];
    
    Image image;
    
    for (uint8_t i = 0; i < 23; i += 1) {
        snprintf(buffer, 64, "%d.png", i);

        image = LoadImage(buffer);
        textures[i] = LoadTextureFromImage(image);
        UnloadImage(image);
    }
    
    screenA = LoadRenderTexture(RENDER_FRAME_WIDTH, RENDER_FRAME_HEIGHT);
    screenB = LoadRenderTexture(RENDER_FRAME_WIDTH, RENDER_FRAME_HEIGHT);
}

void MAIN_CloseVideo() {
    CloseWindow();
}

void MAIN_UpdateFrame(struct _Frame * frame) {
    ClearBackground(BLACK);
    
    for (uint16_t x = 0; x < RENDER_FRAME_WIDTH; x += 1) {
        DrawRectangle(x, RENDER_FRAME_HEIGHT / 2 - frame->wallBuffer[x], 1, frame->wallBuffer[x] * 2, (Color){frame->wallBuffer[x], frame->wallBuffer[x], frame->wallBuffer[x], 255});
    }
    
    for (uint8_t i = 0; i < frame->spriteCount; i += 1) {
        int16_t spriteX = frame->sprites[i].x;
        int16_t spriteY = frame->sprites[i].y;
        
        if (frame->sprites[i].centered) {
            spriteX -= RENDER_SPRITE_SIZE / 2;
            spriteY -= RENDER_SPRITE_SIZE / 2;
        }
    
        DrawTextureEx(textures[frame->sprites[i].index], (Vector2){spriteX, spriteY},  0.0f, (float)frame->sprites[i].size, WHITE);
    }
    
    frame->spriteCount = 0;
}

void MAIN_UpdateFrames(struct _Frame * frameA, struct _Frame * frameB) {
    BeginTextureMode(screenA);
    MAIN_UpdateFrame(frameA);
    EndTextureMode();
    
    BeginTextureMode(screenB);
    MAIN_UpdateFrame(frameB);
    EndTextureMode();

    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexturePro(
        screenA.texture,
        (Rectangle){ 0, 0, 640, -360 },
        (Rectangle){ 0, 0, RENDER_FRAME_WIDTH, RENDER_FRAME_HEIGHT},
        (Vector2){ 0, 0 },
        0.0f,
        WHITE
    );
    DrawTexturePro(
        screenB.texture,
        (Rectangle){0, 0, 640, -360 },
        (Rectangle){RENDER_FRAME_WIDTH, 0, RENDER_FRAME_WIDTH, RENDER_FRAME_HEIGHT},
        (Vector2){0, 0},
        0.0f,
        WHITE
    );
    EndDrawing();
    
    /*
    FILE * file = fopen("data.bin", "wb");
     
    fwrite(frameA->wallBuffer, sizeof(int8_t), RENDER_FRAME_WIDTH, file);
     
    for (uint8_t i = 0; i < frameA->spriteCount; i += 1) {
        uint16_t spriteX = frameA->sprites[i].x + 64;
        uint16_t spriteY = frameA->sprites[i].y + 64;

        if (frameA->sprites[i].centered) {
            spriteX -= RENDER_SPRITE_SIZE / 2;
            spriteY -= RENDER_SPRITE_SIZE / 2;
        }
        uint32_t packedSprite = 0;

        packedSprite |= (uint32_t)(spriteX & 0x3FF) << 22;
        packedSprite |= (uint32_t)(spriteY & 0x1FF) << 13;
        packedSprite |= (uint32_t)(frameA->sprites[i].index & 0x1F) << 8;
        packedSprite |= (uint32_t)(frameA->sprites[i].size & 0x07) << 5;
        
        fwrite(&packedSprite, sizeof(uint32_t), 1, file);
    }
    
    if (frameA->spriteCount < 128) {
        uint32_t zeroSprite = 0;
        for (uint8_t i = frameA->spriteCount; i < 128; i++) {
            fwrite(&zeroSprite, sizeof(uint32_t), 1, file);
        }
    }
     
    fclose(file);
    */
}

void MAIN_PollInput(struct _Player * playerA, struct _Player * playerB) {
    if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_LEFT)) {
        activePlayer = !activePlayer;
    }
    
    playerA->input.buttonState = 0; 
    playerB->input.buttonState = 0; 

    struct _Player * player = playerA;
    if (activePlayer) {
        player = playerB;
    }

    if (IsGamepadButtonPressed(0, GAMEPAD_BUTTON_RIGHT_FACE_DOWN)) {
        player->input.buttonState = 1;
    }
    
    float leftStickX = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_X);
    float leftStickY = GetGamepadAxisMovement(0, GAMEPAD_AXIS_LEFT_Y);
    float rightStickX = GetGamepadAxisMovement(0, GAMEPAD_AXIS_RIGHT_X);
    
    if (leftStickX > -0.1 && leftStickX < 0.1) leftStickX = 0.0f;
    if (leftStickY > -0.1 && leftStickY < 0.1) leftStickY = 0.0f;
    if (rightStickX > -0.1 && rightStickX < 0.1) rightStickX = 0.0f;
    
    player->input.moveX = (int8_t)(leftStickX * 127);
    player->input.moveY = (int8_t)(leftStickY * 127);
    player->input.viewX = (int8_t)(rightStickX * 127);
}

int main() {    
    MAIN_InitVideo();
    
    struct _Player playerA;
    struct _Player playerB;

    struct _Frame frameA;
    struct _Frame frameB;
    
    struct _Map * map = MAP_SelectRandom();
    PLAYER_SetPosition(&playerA, map->playerPositionA, map->playerDirectionA);
    PLAYER_SetPosition(&playerB, map->playerPositionB, map->playerDirectionB);
    PLAYER_SetSpriteIndex(&playerA, 0);
    PLAYER_SetSpriteIndex(&playerB, 2);

    enum _state state = STATE_MAIN_MENU;
    enum _state lastState = STATE_END_SCREEN;
    enum _state nextState = STATE_MAIN_MENU;
    
    while (1) {
        MAIN_PollInput(&playerA, &playerB);
    
        switch (state) {
            case STATE_MAIN_MENU:
                if (state != lastState) {
                    PLAYER_SetReady(&playerA, 0);
                    PLAYER_SetReady(&playerB, 0);
                }
                
                RENDER_DrawMenu(&playerA, map, &frameA);
                RENDER_DrawMenu(&playerB, map, &frameB);
                
                if (playerA.input.buttonState) {PLAYER_SetReady(&playerA, !PLAYER_GetReady(&playerA));}
                if (playerB.input.buttonState) {PLAYER_SetReady(&playerB, !PLAYER_GetReady(&playerB));}

                if (PLAYER_GetReady(&playerA) && PLAYER_GetReady(&playerB)) {
                    nextState = STATE_PLAYING;
                }

                break;
            case STATE_PLAYING:
                if (state != lastState) {
                    map = MAP_SelectRandom();
                    PLAYER_SetPosition(&playerA, map->playerPositionA, map->playerDirectionA);
                    PLAYER_SetPosition(&playerB, map->playerPositionB, map->playerDirectionB);
                    PLAYER_ResetHealth(&playerA);
                    PLAYER_ResetHealth(&playerB);
                }
                
                PLAYER_ApplyMovement(&playerA, map);
                PLAYER_ApplyMovement(&playerB, map);
                PLAYER_ApplyActions(&playerA, &playerB, map);
                PLAYER_ApplyActions(&playerB, &playerA, map);
                
                RENDER_DrawFrame(&playerA, &playerB, map, &frameA);
                RENDER_DrawFrame(&playerB, &playerA, map, &frameB);
                
                if (PLAYER_IsDead(&playerA) || PLAYER_IsDead(&playerB)) {
                    nextState = STATE_END_SCREEN;
                }
                
                break;
            case STATE_END_SCREEN:
                if (state != lastState) {
                    nextState = STATE_MAIN_MENU;
                }
                break;
        }
        
        lastState = state;
        state = nextState;

        MAIN_UpdateFrames(&frameA, &frameB);
    }
    
    MAIN_CloseVideo();

    return 0;
}
