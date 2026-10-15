#include "title.h"
#include "gb_basics.h"
#include "game.h"
#include "bkg_title.h"
#include "mansion_entryway.h"

void title_init(void) {
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

void title_update(void) {
	if (pressed & J_START) scene_change(&scene_entryway);
}

const Scene scene_title = { title_init, title_update, 0 };
