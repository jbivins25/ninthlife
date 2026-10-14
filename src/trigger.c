#include "gb_basics.h"

static Trigger triggers[MAX_TRIGGERS];

Trigger* trigger_add(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t once, void (*on_enter)(Trigger*), void (*on_exit)(Trigger*)) {
	for (uint8_t i = 0; i < MAX_TRIGGERS; i++) {
		if (!triggers[i].active) {
			Trigger* t = &triggers[i];
			t->active = 1; t->inside = 0; t->once = once;
			t->data.box.x = x;
			t->data.box.y = y;
			t->data.box.w = w;
			t->data.box.h = h;
			t->quad = 0;
			t->on_enter = on_enter;
			t->on_exit = on_exit;
			return t;
		}
	}
	return 0;
}

Trigger* trigger_add_quad(quad_points* points, uint8_t once, void (*on_enter)(Trigger*), void (*on_exit)(Trigger*)) {
	for (uint8_t i = 0; i < MAX_TRIGGERS; i++) {
		if (!triggers[i].active) {
			Trigger* t = &triggers[i];
			t->active = 1; t->inside = 0; t->once = once;
			quad_prepare(&t->data.quad, points);
			t->quad = 1;
			t->on_enter = on_enter;
			t->on_exit = on_exit;
			return t;
		}
	}
	return 0;
}

void triggers_clear(void) {
	for (uint8_t i = 0; i < MAX_TRIGGERS; i++) triggers[i].active = 0;
}

uint8_t check_quad_collision(const Actor* a, const quad_data* q) {
	int16_t ax1 = a->x + a->w, ay1 = a->y + a->h;

	if (q->minx > ax1 || q->maxx < a->x || q->miny > ay1 || q->maxy < a->y) return 0;

	int16_t rx = a->x - q->ox, ry = a->y - q->oy;

	for (uint8_t i = 0; i < 4; i++) {
		int16_t base = rx * q->nx[i] + ry * q->ny[i];
		int16_t dx = (int16_t)a->w * q->nx[i];
		int16_t dy = (int16_t)a->h * q->ny[i];

		int16_t amin = base + (dx < 0 ? dx : 0) + (dy < 0 ? dy : 0);
		int16_t amax = base + (dx > 0 ? dx : 0) + (dy > 0 ? dy : 0);

		if (amin > q->qmax[i] || amax < q->qmin[i]) return 0;
	}
	return 1;
}

void triggers_update(const Actor* a) {
	if (!a) return;
	for (uint8_t i = 0; i < MAX_TRIGGERS; i++) {
		Trigger* t = &triggers[i];
		if (!t->active) continue;
		uint8_t hit;
		if (t->quad) {
			quad_data* data = &t->data.quad;
			hit = check_quad_collision(a, data);	
		}
		else {
			bound_box* data = &t->data.box;
			hit = a->x < data->x + data->w && a->x + a->w > data->x && a->y < data->y + data->h && a->y + a->h > data->y;
		}

		if (hit && !t->inside) {
			if (t->once) t->active = 0;
			if (t->on_enter) t->on_enter(t);
		}
		else if (!hit && t->inside) {
			if (t->on_exit) t->on_exit(t);
		}
		t->inside = hit;
	}
}
int16_t iabs16(int16_t v) { return v < 0 ? -v : v; }

void quad_prepare(quad_data *q, const quad_points *pts) {
	q->ox = pts->p[0].x;  q->oy = pts->p[0].y;
	q->minx = q->maxx = pts->p[0].x;
	q->miny = q->maxy = pts->p[0].y;
 
	for (uint8_t i = 0; i < 4; i++) {
		uint8_t n = (i + 1) & 3;
		if (pts->p[i].x < q->minx) q->minx = pts->p[i].x;
		if (pts->p[i].x > q->maxx) q->maxx = pts->p[i].x;
		if (pts->p[i].y < q->miny) q->miny = pts->p[i].y;
		if (pts->p[i].y > q->maxy) q->maxy = pts->p[i].y;
 
		int16_t ex = pts->p[n].x - pts->p[i].x;
		int16_t ey = pts->p[n].y - pts->p[i].y;
		int16_t nx = -ey, ny = ex;
 
		while (iabs16(nx) > 63 || iabs16(ny) > 63) { nx >>= 1; ny >>= 1; }
		q->nx[i] = (int8_t)nx;
		q->ny[i] = (int8_t)ny;
	}
 
	for (uint8_t i = 0; i < 4; i++) {
		int16_t lo = 0, hi = 0;
		for (uint8_t j = 1; j < 4; j++) {
			int16_t p = (pts->p[j].x - q->ox) * q->nx[i] + (pts->p[j].y - q->oy) * q->ny[i];
			if (p < lo) lo = p;
			if (p > hi) hi = p;
		}
		q->qmin[i] = lo;
		q->qmax[i] = hi;
	}
}
