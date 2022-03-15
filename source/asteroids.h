#ifndef ASTEROIDS_H
#define ASTEROIDS_H

#include "common.h"
#include "sprites.h"
#include "shots.h"
#include "player.h"
#include "messages.h"

struct Asteroids {
    struct Asteroid *first_asteroid;
};

struct Asteroid {
    struct Asteroid *previous;
    struct Asteroid *next;
    struct Sprite sprite;
    unsigned short size;
};

enum Errors asteroid_create(struct Game *ast, struct Asteroids *asteroids, enum Sprites size, short angle, float spin, float speed);
enum Errors asteroid_populate(struct Asteroids *asteroids, struct Game *ast);
void asteroid_delete(struct Asteroid *delete_asteroid, struct Asteroids *asteroids);
void asteroid_delete_all(struct Asteroids *asteroids);
enum Errors asteroids_update(SDL_Renderer *rend, struct Asteroids *asteroids);
enum Errors asteroid_collision_check(struct Game *ast, struct Player *player, struct Shots *shots, struct Asteroids *asteroids, struct Messages *msg);
enum Errors asteroid_split(struct Game *ast, struct Asteroids *asteroids, struct Asteroid *asteroid, float angle);
enum Errors asteroids_render(SDL_Renderer *rend, struct Asteroids *asteroids);

#endif
