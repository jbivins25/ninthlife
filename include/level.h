#ifndef LEVEL_H
#define LEVEL_H
#include <gb/gb.h>

#define SOLID_NORMAL	0x01
#define SOLID_GHOST	0x02
#define INTER_NORMAL	0x04
#define INTER_GHOST	0x08

typedef struct {
	uint8_t w, h;
	const uint8_t* map;
} Level;

uint8_t level_blocked(int16_t x, int16_t y, uint8_t w, uint8_t h);
uint8_t level_interactable(uint8_t direction, int16_t x, int16_t y, uint8_t w, uint8_t h);

#endif
