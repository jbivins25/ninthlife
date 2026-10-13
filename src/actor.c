#include "gb_basics.h"

static Actor actors[MAX_ACTORS];
Actor* player = 0;

Actor* actor_spawn(uint8_t tile, uint8_t palette, int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t is_moving, uint8_t direction) {
	for (uint8_t i = 0; i < MAX_ACTORS; i++) {
		if (!actors[i].active) {
			Actor* a = &actors[i];
			a->active = 1;
			a->x = x; a->y = y; a->w = w; a->h = h;
			a->tile = tile;
			a->update = 0;
			a->is_moving = is_moving;
			a->direction = direction;
			a->anim_frame = 0;
			a->anim_timer = 0;
			set_sprite_tile(i, tile);
			set_sprite_prop(i, palette & 0x07);
			return a;
		}
	}
	return 0;
}

void actor_kill(Actor* a) {
	a->active = 0;
	hide_sprite(a - actors);
}

void actors_clear(void) {
	for (uint8_t i = 0; i < MAX_ACTORS; i++) {
		actors[i].active = 0;
		hide_sprite(i);
	}
	player = 0;
}

void actors_update(void) {
	for (uint8_t i = 0; i < MAX_ACTORS; i++) {
		if (actors[i].active && actors[i].update) actors[i].update(&actors[i]);
		if (actors[i].active && actors[i].is_moving && actors[i].animate) actors[i].animate(&actors[i]);
	}
}

void actors_draw(void) {
	for (uint8_t i = 0; i < MAX_ACTORS; i++) {
		if (actors[i].active) move_sprite(i, actors[i].x + 8, actors[i].y + 16);
	}
}

uint8_t actor_overlap(const Actor* a, const Actor* b) {
	return a->x < b->x + b->w && a->x + a->w > b->x && a->y < b->y + b->h && a->y + a->h > b->y;
}
