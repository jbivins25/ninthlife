#ifndef PLAYER_H
#define PLAYER_H
#include "game.h"
#include "gb_basics.h"
#include "vision.h"
#include "spr_player.h"

extern Actor* player;
extern uint8_t vision;
extern uint8_t in_light;

void player_init(void);
void player_update(Actor* self);
void player_animate(Actor* self);
void light_trigger(Trigger* t);

#endif
