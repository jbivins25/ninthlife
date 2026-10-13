#ifndef GAME_H
#define GAME_H
#include <stdint.h>

extern uint8_t keys, prev_keys, pressed;

void input_update(void);

#endif
