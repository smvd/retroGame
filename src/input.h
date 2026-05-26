#ifndef INCLUDED_INPUT_HEADER

#include <stdint.h>

struct _Input {
    uint8_t moveX;
    uint8_t moveY;
    uint8_t viewX;
    uint8_t buttonState;
};

void INPUT_Reset(struct _Input * input);
uint8_t INPUT_ButtonPressed(struct _Input * input);

#endif
