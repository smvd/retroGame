#ifndef INCLUDED_WORLD_HEADER
#define INCLUDED_WORLD_HEADER

#include <stdint.h>

#include <stdio.h>

#include "fx.h"

#define BTN_W      0x01
#define BTN_A      0x02
#define BTN_S      0x04
#define BTN_D      0x08
#define BTN_SPACE  0x10

extern uint8_t world[8][8];
extern struct _FX_Vector player;
extern struct _FX_Vector camera;
extern struct _FX_Vector direction;
extern uint8_t buttons;

void WORLD_Reset();
void WORLD_UpdatePlayer();

#endif
