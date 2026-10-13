#include <gb/gb.h>
#include <gb/cgb.h>
#include <gbdk/console.h>
#include <gbdk/font.h>
#include <stdio.h>
#include "gb_basics.h"
#include "game.h"
#include "bkg_title.h"
#include "player.h"

extern const Scene scene_title, scene_play, scene_gameover;

static uint8_t score;

static const uint8_t ball[] = {
	0x3C,0x3C, 0x7E,0x7E, 0xFF,0xFF, 0xFF,0xFF,
	0xFF,0xFF, 0xFF,0xFF, 0x7E,0x7E, 0x3C,0x3C
};
static const palette_color_t pal_a[] = { RGB(31,31,31), RGB(20,25,31), RGB(5,10,25), RGB(0,0,0) };
static const palette_color_t pal_b[] = { RGB(31,31,31), RGB(31,25,10), RGB(25,8,2),  RGB(0,0,0) };

static void title_init(void) {
	set_bkg_data(bkg_title_TILE_ORIGIN, bkg_title_TILE_COUNT, bkg_title_tiles);

	if (_cpu == CGB_TYPE) {
		set_bkg_palette(0, bkg_title_PALETTE_COUNT, bkg_title_palettes);
		VBK_REG = VBK_ATTRIBUTES;
		set_bkg_tiles(0, 0, bkg_title_MAP_ATTRIBUTES_WIDTH, bkg_title_MAP_ATTRIBUTES_HEIGHT, bkg_title_map_attributes);
		VBK_REG = VBK_TILES;
	}

	set_bkg_tiles(0, 0, bkg_title_MAP_ATTRIBUTES_WIDTH, bkg_title_MAP_ATTRIBUTES_HEIGHT, bkg_title_map);
	SHOW_BKG;
}

static void title_update(void) {
	if (pressed & J_START) scene_change(&scene_play);
}

const Scene scene_title = { title_init, title_update, 0 };

static void hit_hazard(Trigger* t) { (void*)t; scene_change(&scene_gameover); }

static void play_init(void) {
	cls();
	score = 0;
	player_init();
	SHOW_SPRITES;
	trigger_add(150, 0, 10, 144, 0, hit_hazard);
}

const Scene scene_play = { play_init, 0, 0 };

static void gameover_init(void) {
	cls();
	gotoxy(5, 8); printf("GAME OVER");
	gotoxy(5, 10); printf("SCORE: %u", (unsigned)score);
}

static void gameover_update(void) {
	if (pressed & J_START) scene_change(&scene_title);
}

const Scene scene_gameover = { gameover_init, gameover_update, 0 };

void main(void) {
	font_init();
	font_set(font_load(font_ibm));
	if (_cpu == CGB_TYPE) {
		static const palette_color_t bkg_pal[] = {
			RGB(31,31,31), RGB(20,20,20), RGB(10,10,10), RGB(0,0,0)
		};
		set_bkg_palette(0, 1, bkg_pal);
	}
	SHOW_BKG;
	scene_run(&scene_title);
}
