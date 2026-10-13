#include <gb/gb.h>
#include "game.h"

uint8_t keys, prev_keys, pressed;

void input_update(void) {
	prev_keys = keys;
	keys = joypad();
	pressed = keys & ~prev_keys;
}
