#include <gb/gb.h>
#include <gb/cgb.h>
#include <gbdk/console.h>
#include <gbdk/font.h>
#include <stdio.h>
#include "gb_basics.h"
#include "game.h"
#include "player.h"
#include "title.h"
#include "level.h"

extern const Scene scene_gameover;

static void gameover_init(void) {
	cls();
	gotoxy(5, 8); printf("GAME OVER");
}

static void gameover_update(void) {
	if (pressed & J_START) scene_change(&scene_title);
}

const Scene scene_gameover = { gameover_init, gameover_update, 0 };

uint8_t gamemode = MODE_EXPLORE;
Level curr_level = {0};

void main(void) {
	scene_run(&scene_title);
}
