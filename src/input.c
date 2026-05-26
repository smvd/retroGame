#include "input.h"
#include "bitwise.h"

void INPUT_Reset(struct _Input * input) {
    input->moveX = 0;
    input->moveY = 0;
    input->viewX = 0;
    input->buttonState = 0;
}

uint8_t INPUT_ButtonPressed(struct _Input * input) {
    return (GET_BIT(input->buttonState, 1) == 1);
}
