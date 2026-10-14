#include <gb/gb.h>
#include <gb/cgb.h>
#include <gbdk/console.h>
#include "gb_basics.h"
#include "bkg_entryway.h"
#include "player.h"
#include "mansion_entryway.h"
#include "vision.h"

void entryway_init(void) {
	cls();
	set_bkg_data(bkg_entryway_TILE_ORIGIN, bkg_entryway_TILE_COUNT, bkg_entryway_tiles);

	if (_cpu == CGB_TYPE) {
		set_bkg_palette(0, bkg_entryway_PALETTE_COUNT, bkg_entryway_palettes);
		VBK_REG = VBK_ATTRIBUTES;
		set_bkg_tiles(0, 0, bkg_entryway_MAP_ATTRIBUTES_WIDTH, bkg_entryway_MAP_ATTRIBUTES_HEIGHT, bkg_entryway_map_attributes);
		VBK_REG = VBK_TILES;
	}

	set_bkg_tiles(0, 0, bkg_entryway_MAP_ATTRIBUTES_WIDTH, bkg_entryway_MAP_ATTRIBUTES_HEIGHT, bkg_entryway_map);
	SHOW_BKG;
	player_init();
	quad_points points;
	points.p[0].x = 8;
	points.p[0].y = 80;
	points.p[1].x = 54;
	points.p[1].y = 67;
	points.p[2].x = 54;
	points.p[2].y = 110;
	points.p[3].x = 8;
	points.p[3].y = 119;
	trigger_add_quad(&points, 0, light_trigger, light_trigger);
	SHOW_SPRITES;
}

static const palette_color_t entryway_ghost[bkg_entryway_TOTAL_COLORS] = {
	RGB8(  0,181,214), RGB8(  0, 99,115), RGB8(  0, 16, 33), RGB8(  0,  0,  0),
	RGB8( 66,255,255), RGB8(  0,181,214), RGB8(  0, 99,115), RGB8(  0, 16, 33),
	RGB8( 64,163,179), RGB8( 40,139,155), RGB8(  0, 99,115), RGB8(  0,  0,  0),
	RGB8( 99,255,82), RGB8( 66,255, 255), RGB8(  0, 99,115), RGB8(  0,  0,  0)
};

void entryway_vision_apply(void) {
	if (_cpu != CGB_TYPE) return;
	set_bkg_palette(0, bkg_entryway_PALETTE_COUNT, vision == VIS_GHOST ? entryway_ghost : bkg_entryway_palettes);
}

const Scene scene_entryway = { entryway_init, entryway_vision_apply, 0 };

