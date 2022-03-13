#include "shots.h"

enum Errors shot_create(struct Shots *shots, struct Sprite *player, struct Game *ast) {
    for (unsigned short i = 0; i < shots->shotc; i++) {
        if (!shots->shotv[i].sprite.text) {
            Mix_PlayChannel(-1, ast->laser, 0);
            shots->shotv[i].lifetime = 0;
            enum Errors rvalue = 0;

            rvalue = sprite_create(&shots->shotv[i].sprite, shot, ast);
            if (rvalue)
                return rvalue;

            shots->shotv[i].sprite.radius = SHOT_RADIUS;
            shots->shotv[i].shot_count = shots->shots_count;
            if (shots->shots_count)
                shots->shots_count -= 1;

            // Set shot angle to player angle and rotate shot to new angle
            shots->shotv[i].sprite.angle = player->angle;
            rvalue = sprite_rotate(&shots->shotv[i].sprite, ast->rend);
            if (rvalue)
                return rvalue;

            // Set the x and y velocity based on player angle.
            float radian = player->angle * PI / 180;
            shots->shotv[i].sprite.y_vel = cos(radian) * -SHOT_VELOCITY;
            shots->shotv[i].sprite.x_vel = sin(radian) * SHOT_VELOCITY;

            // Center shot to player sprite and move it forwared 3 steps.
            pos_set(&shots->shotv[i].sprite, center_x, pos_get(player, center_x));
            pos_set(&shots->shotv[i].sprite, center_y, pos_get(player, center_y));
            shots->shotv[i].sprite.x += shots->shotv[i].sprite.x_vel * 3;
            shots->shotv[i].sprite.rect.x = (int)shots->shotv[i].sprite.x;
            shots->shotv[i].sprite.y += shots->shotv[i].sprite.y_vel * 3;
            shots->shotv[i].sprite.rect.y = (int)shots->shotv[i].sprite.y;
            return 0;
        }
    }
    return 0;
}

void shots_update(struct Shots *shots) {
    for (unsigned short i = 0; i < shots->shotc; i++) {
        if (shots->shotv[i].sprite.text) {
            if (shots->shotv[i].lifetime < SHOT_LIFE ) {
                shots->shotv[i].lifetime++;
                shots->shotv[i].sprite.x += shots->shotv[i].sprite.x_vel;
                shots->shotv[i].sprite.y += shots->shotv[i].sprite.y_vel;
                offscreen_check(&shots->shotv[i].sprite);
                shots->shotv[i].sprite.rect.x = (int)shots->shotv[i].sprite.x;
                shots->shotv[i].sprite.rect.y = (int)shots->shotv[i].sprite.y;
            } else {
                SDL_DestroyTexture(shots->shotv[i].sprite.text);
                shots->shotv[i].sprite.text = NULL;
            }
        }
    }
}

void shots_delete_all(struct Shots *shots) {
    for (unsigned short i = 0; i < shots->shotc; i++) {
        if (shots->shotv[i].sprite.text) {
            SDL_DestroyTexture(shots->shotv[i].sprite.text);
            shots->shotv[i].sprite.text = NULL;
        }
    }
}

enum Errors shots_render(SDL_Renderer *rend, struct Shots *shots) {
    for (unsigned short i = 0; i < shots->shotc; i++) {
        if (shots->shotv[i].sprite.text) {
            enum Errors rvalue = SDL_RenderCopy(rend, shots->shotv[i].sprite.text, NULL, &shots->shotv[i].sprite.rect);
            if (rvalue)
                return rvalue;
        }
    }
    return 0;
}
