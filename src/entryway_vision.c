#include <gb/gb.h>
#include <gb/cgb.h>
#include "bkg_entryway.h"
#include "vision.h"
#include "player.h"

static const palette_color_t entryway_ghost[bkg_entryway_TOTAL_COLORS] = {
	RGB8(  0,181,214), RGB8(  0, 99,115), RGB8(  0, 16, 33), RGB8(  0,  0,  0),
	RGB8( 66,255,255), RGB8(  0,181,214), RGB8(  0, 99,115), RGB8(  0, 16, 33),
	RGB8( 64,163,179), RGB8( 40,139,155), RGB8(  0, 99,115), RGB8(  0,  0,  0),
	RGB8( 66,255,255), RGB8( 99,255, 82), RGB8(  0, 99,115), RGB8(  0,  0,  0)
};

void entryway_vision_apply(void) {
	if (_cpu != CGB_TYPE) return;
	set_bkg_palette(0, bkg_entryway_PALETTE_COUNT, vision == VIS_GHOST ? entryway_ghost : bkg_entryway_palettes);
}
