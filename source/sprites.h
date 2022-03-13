#ifndef SPRITES_H
#define SPRITES_H

#include "common.h"

// Create a struct for game sprites.
struct Sprite {
    SDL_Texture* orig_text;
    SDL_Texture* text;
    SDL_Rect orig_rect;
    SDL_Rect rect;
    unsigned short radius;
    unsigned short tex_radius;
    float angle;
    float spin;
    float x, y, x_vel,y_vel;
    bool update;
};

enum Errors sprite_create(struct Sprite *sprite, enum Sprites image, struct Game *ast);
enum Errors sprite_update(SDL_Renderer *rend, struct Sprite *sprite);
enum Errors sprite_rotate(struct Sprite *sprite, SDL_Renderer *rend);
float pos_get(struct Sprite *sprite, enum Positions pos);
void pos_set(struct Sprite *sprite, enum Positions pos, float new_pos);
void offscreen_check(struct Sprite *sprite);
bool collision_check(struct Sprite *sprite1, struct Sprite *sprite2);

#endif
