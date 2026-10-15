#include <gb/gb.h>
#include "level.h"
#include "player.h"

uint8_t level_solid_at(int16_t px, int16_t py) {
	if (px < 0 || py < 0) return 1;
	uint8_t tx = px >> 2, ty = py >> 2;
	if (tx >= curr_level.w || ty >= curr_level.h) return 1;
	uint8_t mask = (vision == VIS_GHOST) ? SOLID_GHOST : SOLID_NORMAL;
	return curr_level.map[ty*curr_level.w + tx] & mask;
}

uint8_t level_blocked(int16_t x, int16_t y, uint8_t w, uint8_t h) {
	return level_solid_at(x, y) || level_solid_at(x + w - 1, y) || level_solid_at(x, y + h - 1) || level_solid_at(x + w - 1, y + h - 1);
}

uint8_t level_interactable(uint8_t direction, int16_t x, int16_t y, uint8_t w, uint8_t h) {
	if (x < 0 || y < 0) return 0;
	uint8_t tx1 = 0, ty1 = 0, tx2 = 0, ty2 = 0;
	switch (direction) {
		case (NORTH):
			tx1 = x >> 3;
			ty1 = (y - 2) >> 3; 
			tx2 = (x + w - 1) >> 3;
			ty2 = ty1;
			break;
		case (SOUTH):
			tx1 = x >> 3;
			ty1 = (y + h + 1) >> 3; 
			tx2 = (x + w - 1) >> 3;
			ty2 = ty1;
			break;
		case (WEST):
			tx1 = (x - 2) >> 3;
			ty1 = y >> 3; 
			tx2 = tx1;
			ty2 = (y + h - 1) >> 3;
			break;
		case (EAST):
			tx1 = (x + w + 1) >> 3;
			ty1 = y >> 3; 
			tx2 = tx1;
			ty2 = (y + h - 1) >> 3;
			break;
	}

	uint8_t mask = (vision == VIS_GHOST) ? INTER_GHOST : INTER_NORMAL;
	uint8_t hit = 0;
	if (tx1 < curr_level.w && ty1 < curr_level.h) hit = curr_level.map[ty1*curr_level.w + tx1] & mask;
	if (!hit && tx2 < curr_level.w && ty2 < curr_level.h) hit = curr_level.map[ty2*curr_level.w + tx2] & mask;
	return hit;
}
