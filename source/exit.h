#ifndef EXIT_H
#define EXIT_H

#include "common.h"
#include "sprites.h"
#include "shots.h"
#include "astroids.h"

void memory_release_exit(struct Game *ast, struct Player *player, struct Shots *shots, struct Astroids *astroids, struct Messages *msg);

#endif
