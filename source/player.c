#include "player.h"

enum Errors player_create(struct Player *player, struct Game *ast) {
    // Get the rectangles from the 2 ship images and find the longest_side.
    unsigned short longest_side = 0;
    for (unsigned short i = 0; i < LEN(player->orig_text); i++) {

        player->orig_text[i] = ast->textv[i];
        if (!player->orig_text[i])
            return ERROR_NIMG;

        if (SDL_QueryTexture(player->orig_text[i], NULL, NULL, &player->orig_rect[i].w, &player->orig_rect[i].h))
            return ERROR_QTEX;

        if ( longest_side < player->orig_rect[i].w ) {
            longest_side = player->orig_rect[i].w;
        } else if ( longest_side < player->orig_rect[i].h ) {
            longest_side = player->orig_rect[i].h;
        }
    }
    player->sprite.radius = SHIP_RADIUS;
    player->sprite.rect.w = longest_side;
    player->sprite.rect.h = longest_side;
    player->sprite.tex_radius = longest_side / 2;

    // Create Texture to draw and rotate the player textures onto.
    player->sprite.text = SDL_CreateTexture( ast->rend,
                                      SDL_PIXELFORMAT_RGBA8888,
                                      SDL_TEXTUREACCESS_TARGET,
                                      player->sprite.rect.w,
                                      player->sprite.rect.h );
    if (!player->sprite.text)
        return ERROR_TEXT;

    // Make the texture transparent so only the player is seen.
    if (SDL_SetTextureBlendMode(player->sprite.text, SDL_BLENDMODE_BLEND))
        return ERROR_BLND;

    // Center the orignal rects to the center of the player.tex, only player.rect will move.
    for (int i = 0; i < 2; i++) {
        player->orig_rect[i].x = (player->sprite.rect.w - player->orig_rect[i].w) / 2;
        player->orig_rect[i].y = (player->sprite.rect.h - player->orig_rect[i].h) / 2;
    }
    player->sprite.orig_text = player->orig_text[0];
    player->sprite.orig_rect = player->orig_rect[0];

    enum Errors rvalue = player_reset(player, ast);
    if (rvalue)
        return rvalue;
    return 0;
}

enum Errors player_reset(struct Player *player, struct Game *ast) {
    // Reset flags and angle.
    player_immunity_toggle(player, true);
    player->sprite.angle = 0;
    // Center player on the screen.
    pos_set(&player->sprite, center_x, WIDTH / (float)2);
    pos_set(&player->sprite, center_y, HEIGHT / (float)2);
    player->sprite.rect.x = player->sprite.x;
    player->sprite.rect.y = player->sprite.y;
    player->sprite.x_vel = 0;
    player->sprite.y_vel = 0;

     enum Errors rvalue = sprite_rotate(&player->sprite, ast->rend);
    if (rvalue)
        return rvalue;
    return 0;
}

void player_engine_toggle(struct Player *player, struct Game *ast) {
    if (player->engine) {
        player->engine = false;
        player->sprite.orig_text = player->orig_text[player->engine];
        player->sprite.orig_rect = player->orig_rect[player->engine];
        Mix_HaltChannel(ast->thrust_channel);
    } else {
        player->engine = true;
        player->sprite.orig_text = player->orig_text[player->engine];
        player->sprite.orig_rect = player->orig_rect[player->engine];
        ast->thrust_channel = Mix_PlayChannel(-1, ast->thrusters, -1);
    }
    player->sprite.update = true;
}

void player_thrust_update(struct Sprite *player) {
        float radian = player->angle * PI / 180;
        player->x_vel += sin(radian) * THRUST;
        player->y_vel -= cos(radian) * THRUST;
}

enum Errors player_update(SDL_Renderer *rend, struct Player *player) {
    enum Errors rvalue = sprite_update(rend, &player->sprite);
    if (rvalue)
        return rvalue;
    player->sprite.update = false;
    if (player->immunity) {
        if (player->immunity_counter > 9)
            player->immunity_counter = 0;
        else
            player->immunity_counter += 1;
    }
    return 0;
}

enum Errors player_render(SDL_Renderer *rend, struct Player *player) {
    if (!player->immunity || player->immunity_counter > 4)
        if (SDL_RenderCopy(rend, player->sprite.text, NULL, &player->sprite.rect))
            return ERROR_COPY;
    return 0;
}

void player_immunity_toggle(struct Player *player, bool toggle) {
    if (toggle) {
        player->immunity = true;
        SDL_RemoveTimer(player->immunity_timer);
        player->immunity_timer = SDL_AddTimer(3000, player_immunity, NULL);
        player->immunity_counter = 0;

    } else {
        player->immunity = false;
    }
}

// Create a callback event for immunity.
Uint32 player_immunity() {
    SDL_Event event;
    event.type = SDL_USEREVENT;
    event.user.code = TIMER_IMMUNE;
    event.user.data1 = NULL;
    event.user.data2 = NULL;
    SDL_PushEvent(&event);
    return 0;
}
