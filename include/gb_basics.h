#ifndef GB_BASICS
#define GB_BASICS
#include <gb/gb.h>
#include <stdint.h>

//Game state
typedef enum { STATE_TITLE, STATE_PLAY, STATE_GAMEOVER } GameState;

//Actors
#define MAX_ACTORS 16

typedef enum { NORTH, SOUTH, EAST, WEST } Direction;

typedef struct Actor Actor;
struct Actor {
	uint8_t active;
	int16_t x, y;
	uint8_t w, h;
	uint8_t tile;
	uint8_t is_moving;
	uint8_t anim_frame;
	uint8_t anim_timer;
	uint8_t direction;
	void (*update)(Actor* self);
	void (*animate)(Actor* self);
};

Actor* actor_spawn(uint8_t tile, uint8_t palette, int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t is_moving, uint8_t direction);
void actor_kill(Actor* a);
void actors_clear(void);
void actors_update(void);
void actors_draw(void);
uint8_t actor_overlap(const Actor* a, const Actor* b);

//Triggers
#define MAX_TRIGGERS 8

struct Vector2 {
	int16_t x, y;
};

typedef struct {
	struct Vector2 p[4];
} quad_points;

typedef struct {
	int16_t minx, miny, maxx, maxy; //normal bounding box
	int16_t ox, oy; // origin
	int8_t  nx[4], ny[4]; // scaled edge normals
	int16_t qmin[4], qmax[4]; // quad's projected range per axis
} quad_data;

typedef struct {
	uint16_t x, y;
	uint8_t w, h;
} bound_box;

void quad_prepare(quad_data *q, const quad_points *pts);

typedef struct Trigger Trigger;
struct Trigger {
	uint8_t active, inside, once, quad;
	union {
		bound_box box;
		quad_data quad;
	} data;
	void (*on_enter)(Trigger* self);
	void (*on_exit)(Trigger* self);
};

Trigger* trigger_add(int16_t x, int16_t y, uint8_t w, uint8_t h, uint8_t once, void (*on_enter)(Trigger *), void (*on_exit)(Trigger *));
Trigger* trigger_add_quad(quad_points* points, uint8_t once, void (*on_enter)(Trigger *), void (*on_exit)(Trigger *));
void triggers_clear(void);
void triggers_update(const Actor* a);

//Scenes
typedef struct {
	void (*init)(void);
	void (*update)(void);
	void (*cleanup)(void);
} Scene;

void scene_change(const Scene* next);
void scene_run(const Scene* first);

#endif
