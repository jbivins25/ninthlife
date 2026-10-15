#include <gb/gb.h>
#include <gb/cgb.h>
#include "player.h"
#include "level.h"

Actor* player = 0;
uint8_t vision = VIS_NORM;
uint8_t in_light = 0;
const palette_color_t player_light_palette[4] = {
	RGB8(0,0,0), RGB8(222,176,44), RGB8(56,56,64), RGB8(0,0,0)	
};

typedef enum {
	TILE_SOUTH_SIT = 0,
	TILE_SOUTH_W1,
	TILE_SOUTH_W2,
	TILE_EAST_SIT,
	TILE_EAST_W1,
	TILE_EAST_W2,
	TILE_NORTH_SIT,
	TILE_NORTH_W1,
	TILE_NORTH_W2
} player_face;

void player_init(void) {
	set_sprite_data(0, spr_player_TILE_COUNT, spr_player_tiles);
	if (_cpu == CGB_TYPE) set_sprite_palette(0, spr_player_PALETTE_COUNT, spr_player_palettes);
	player = actor_spawn(0, 0, 80, 72, 8, 7, 0, 0);
	player->update = player_update;
	player->animate = player_animate;
	vision = VIS_NORM;
}

void player_update(Actor* self) {
	static uint8_t prev_light = 0;
	int16_t nx = self->x, ny = self->y;
	if (keys & J_LEFT) {
		nx--;
		self->is_moving = 1;
		self->direction = WEST;
	}
	else if (keys & J_RIGHT) {
		nx++;
		self->is_moving = 1;
		self->direction = EAST;
	}
	else if (keys & J_UP) {
		ny--;
		self->is_moving = 1;
		self->direction = NORTH;
	}
	else if (keys & J_DOWN) {
		ny++;
		self->is_moving = 1;
		self->direction = SOUTH;
	}
	else {
		self->is_moving = 0;
	}
	if (!level_blocked(nx, self->y, self->w, self->h)) self->x = nx;
	if (!level_blocked(self->x, ny, self->w, self->h)) self->y = ny;
	if (pressed & J_B) vision = vision == VIS_NORM ? VIS_GHOST : VIS_NORM;
	if (prev_light != in_light) {
		if (_cpu == CGB_TYPE) in_light ? set_sprite_palette(0, 1, player_light_palette) : set_sprite_palette(0, 1, spr_player_palettes);
		prev_light = in_light;
	}
}

void player_animate(Actor* self) {
	uint8_t tile_to_show = 0;
	uint8_t sprite_flags = 0;

	switch(self->direction) {
		case NORTH:
			tile_to_show = TILE_NORTH_SIT + (self->is_moving ? self->anim_frame+1 : 0);
			break;

		case SOUTH:
			tile_to_show = TILE_SOUTH_SIT + (self->is_moving ? self->anim_frame+1 : 0);
			break;

		case EAST:
			tile_to_show = TILE_EAST_SIT + (self->is_moving ? self->anim_frame+1 : 0);
			break;

		case WEST:
			tile_to_show = TILE_EAST_SIT + (self->is_moving ? self->anim_frame+1 : 0);
			sprite_flags = S_FLIPX;
			break;
	}

	set_sprite_tile(0, tile_to_show);
	set_sprite_prop(0, sprite_flags);
	if (self->is_moving) {
		self->anim_timer++;
		if (self->anim_timer > 8) {
			self->anim_timer = 0;
			self->anim_frame = !self->anim_frame;
		}
	}
	else {
		self->anim_frame = 0;
		self->anim_timer = 0;
	}
}

void light_trigger(Trigger* t) { (void)t; in_light = !in_light; }
