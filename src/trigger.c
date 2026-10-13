#include "gb_basics.h"

static Trigger triggers[MAX_TRIGGERS];

Trigger* trigger_add(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t once, void (*on_enter)(Trigger*)) {
	for (uint8_t i = 0; i < MAX_TRIGGERS; i++) {
		if (!triggers[i].active) {
			Trigger* t = &triggers[i];
			t->active = 1; t->inside = 0; t->once = once;
			t->x = x; t->y = y; t->w = w; t->h = h;
			t->on_enter = on_enter;
			return t;
		}
	}
	return 0;
}

void triggers_clear(void) {
	for (uint8_t i = 0; i < MAX_TRIGGERS; i++) triggers[i].active = 0;
}

void triggers_update(const Actor* a) {
	if (!a) return;
	for (uint8_t i = 0; i < MAX_TRIGGERS; i++) {
		Trigger* t = &triggers[i];
		if (!t->active) continue;
		uint8_t hit = a->x < t->x + t->w && a->x + a->w > t->x && a->y < t->y + t->h && a->y + a->h > t->y;

		if (hit && !t->inside) {
			if (t->once) t->active = 0;
			if (t->on_enter) t->on_enter(t);
		}
		t->inside = hit;
	}
}
