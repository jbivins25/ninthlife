#include <gb/gb.h>
#include <gb/cgb.h>
#include "player.h"

typedef enum {
	TILE_NORTH_SIT = 0,
	TILE_NORTH_W1,
	TILE_NORTH_W2,
	TILE_EAST_SIT,
	TILE_EAST_W1,
	TILE_EAST_W2,
	TILE_SOUTH_SIT,
	TILE_SOUTH_W1,
	TILE_SOUTH_W2
} player_face;

void play_init(void) {
	set_sprite_data(0, spr_player_TILE_COUNT, spr_player_tiles);
	if (_cpu == CGB_TYPE) set_sprite_palette(0, spr_player_PALETTE_COUNT, spr_player_palettes);
	player = actor_spawn(0, 0, 80, 72, 8, 8, 0, 0);
	player->update = player_update;
	player->animate = player_animate;
	vision = VIS_NORM;
}

void player_update(Actor* self) {
	if (keys & J_LEFT) self->x--;
	else if (keys & J_RIGHT) self->x++;
	else if (keys & J_UP) self->y--;
	else if (keys & J_DOWN) self->y++;
	if (pressed & J_B) vision = vision == VIS_NORM ? VIS_GHOST : VIS_NORM;
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
