#ifndef SHOTS_H
#define SHOTS_H

#include "common.h"
#include "sprites.h"

struct Shot {
    struct Sprite sprite;
    unsigned short lifetime;
    unsigned short shot_count;
};

struct Shots {
    struct Shot shotv[SHOT_MAX_COUNT];
    unsigned short shotc;
    unsigned short shots_count;
};

enum Errors shot_create(struct Shots *shots, struct Sprite *player, struct Game *ast);
enum Errors shots_render(SDL_Renderer *rend, struct Shots *shots);
void shots_delete_all(struct Shots *shots);
void shots_update(struct Shots *shots);

#endif
