#include <gb/gb.h>
#include <gb/cgb.h>
#include <stdint.h>

const uint8_t ball[] = {
	0x3c, 0x3c, 0x7e, 0x7e, 0xff, 0xff, 0xff, 0xff,
	0xff, 0xff, 0xff, 0xff, 0x7e, 0x7e, 0x3c, 0x3c
};

const palette_color_t pal[] = {
	RGB(31, 31, 31), RGB(20, 25, 31), RGB(5, 10, 25), RGB(0,0,0)
};

void main(void) {
	uint8_t x = 80, y = 72;

	set_sprite_data(0, 1, ball);
	set_sprite_tile(0, 0);
	if (_cpu == CGB_TYPE) set_sprite_palette(0, 1, pal);
	SHOW_SPRITES;

	while(1) {
		uint8_t keys = joypad();
		if (keys & J_LEFT) x--;
		if (keys & J_RIGHT) x++;
		if (keys & J_UP) y--;
		if (keys & J_DOWN) y++;

		move_sprite(0, x, y);
		vsync();
	}
}
