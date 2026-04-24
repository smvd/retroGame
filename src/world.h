#ifndef INCLUDED_WORLD_HEADER
#define INCLUDED_WORLD_HEADER

#include <math.h>
#include <stdio.h>
#include <stdint.h>

#include "vector.h"

#define BTN_W      0x01
#define BTN_A      0x02
#define BTN_S      0x04
#define BTN_D      0x08
#define BTN_SPACE  0x10

#define WORLD_WIDTH 24
#define WORLD_HEIGHT 24

#define WORLD_MOVE_SPEED 0.001
#define WORLD_TURN_SPEED 0.00174533

extern uint8_t world[WORLD_HEIGHT][WORLD_WIDTH];
extern struct _Vector player;
extern struct _Vector direction;
extern struct _Vector camera;
extern uint8_t buttons;

void WORLD_Reset();
void WORLD_UpdatePlayer();

#endif
