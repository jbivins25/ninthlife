#include "gb_basics.h"
#include "game.h"
#include "player.h"

static const Scene* current = 0;
static const Scene* pending = 0;

void scene_change(const Scene* next) { pending = next; }

void scene_run(const Scene* first) {
	pending = first;
	while(1) {
		if (pending) {
			if (current && current->cleanup) current->cleanup();
			DISPLAY_OFF;
			actors_clear();
			triggers_clear();
			current = pending;
			pending = 0;
			current->init();
			DISPLAY_ON;
		}

		input_update();
		if (current->update) current->update();
		actors_update();
		triggers_update(player);
		actors_draw();
		vsync();
	}
}
