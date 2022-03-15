#ifndef GAME_MODE_H
#define GAME_MODE_H

#include "common.h"
#include "player.h"
#include "shots.h"
#include "asteroids.h"
#include "messages.h"

enum Errors game_mode(struct Game *ast, struct Player *player, struct Shots *shots, struct Asteroids *asteroids, struct Messages *msg);
Uint32 game_mode_timer();

#endif
