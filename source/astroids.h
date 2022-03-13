#ifndef ASTROIDS_H
#define ASTROIDS_H

#include "common.h"
#include "sprites.h"
#include "shots.h"
#include "player.h"
#include "messages.h"

struct Astroids {
    struct Astroid *first_astroid;
};

struct Astroid {
    struct Astroid *previous;
    struct Astroid *next;
    struct Sprite sprite;
    unsigned short size;
};

enum Errors astroid_create(struct Game *ast, struct Astroids *astroids, enum Sprites size, short angle, float spin, float speed);
enum Errors astroid_populate(struct Astroids *astroids, struct Game *ast);
void astroid_delete(struct Astroid *delete_astroid, struct Astroids *astroids);
void astroid_delete_all(struct Astroids *astroids);
enum Errors astroids_update(SDL_Renderer *rend, struct Astroids *astroids);
enum Errors astroid_collision_check(struct Game *ast, struct Player *player, struct Shots *shots, struct Astroids *astroids, struct Messages *msg);
enum Errors astroid_split(struct Game *ast, struct Astroids *astroids, struct Astroid *astroid, float angle);
enum Errors astroids_render(SDL_Renderer *rend, struct Astroids *astroids);

#endif
