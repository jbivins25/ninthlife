#ifndef PLAYER_H
#define PLAYER_H
#include "game.h"
#include "gb_basics.h"
#include "vision.h"
#include "spr_player.h"

Actor* player = 0;
Vision vision = VIS_NORM;

void player_init(void);
void player_update(Actor* self);
void player_animate(Actor* self);

#endif
