#ifndef EXIT_H
#define EXIT_H

#include "common.h"
#include "sprites.h"
#include "shots.h"
#include "asteroids.h"

void memory_release_exit(struct Game *ast, struct Player *player, struct Shots *shots, struct Asteroids *asteroids, struct Messages *msg);

#endif
