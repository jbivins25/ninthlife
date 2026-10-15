#ifndef GAME_H
#define GAME_H
#include <stdint.h>
#include "level.h"

extern uint8_t keys, prev_keys, pressed;
enum GameMode { MODE_EXPLORE, MODE_READING };
extern uint8_t gamemode;
extern Level curr_level;

void input_update(void);

#endif
