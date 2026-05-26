#ifndef INCLUDED_INPUT_HEADER

#include <stdint.h>

struct _Input {
    int8_t moveX;
    int8_t moveY;
    int8_t viewX;
    uint8_t buttonState;
};

void INPUT_Reset(struct _Input * input);
uint8_t INPUT_ButtonPressed(struct _Input * input);

#endif
