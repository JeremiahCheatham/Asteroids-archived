#ifndef PLAYER_H
#define PLAYER_H

#include "common.h"
#include "sprites.h"

struct Player {
    struct Sprite sprite;
    SDL_Texture* orig_text[2];
    SDL_Rect orig_rect[2];
    bool engine;
    bool immunity;
    SDL_TimerID immunity_timer;
    unsigned short immunity_counter;
};

enum Errors player_create(struct Player *player, struct Game *ast);
enum Errors player_reset(struct Player *player, struct Game *ast);
void player_engine_toggle(struct Player *player, struct Game *ast);
void player_thrust_update(struct Sprite *player);
enum Errors player_update(SDL_Renderer *rend, struct Player *player);
enum Errors player_render(SDL_Renderer *rend, struct Player *player);
void player_immunity_toggle(struct Player *player, bool toggle);
Uint32 player_immunity();

#endif
