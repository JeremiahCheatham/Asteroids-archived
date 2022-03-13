#include "sprites.h"


enum Errors sprite_create(struct Sprite *sprite, enum Sprites image, struct Game *ast) {

    // Copy reference to original image to sprite.
    if (ast->textv[image]) {
        sprite->orig_text = ast->textv[image];
    } else {
        return ERROR_NIMG;
    }

    // Get a rect from the original image.
    if (SDL_QueryTexture(sprite->orig_text, NULL, NULL, &sprite->orig_rect.w, &sprite->orig_rect.h))
        return ERROR_QTEX;

    // Find the longest side and create new Texture 1.5 bigger.
    unsigned short longest_side = 0;
    if ( sprite->orig_rect.w > sprite->orig_rect.h ) {
        longest_side = sprite->orig_rect.w;
    } else {
        longest_side = sprite->orig_rect.h;
    }
    sprite->rect.w = longest_side * 1.5;
    sprite->rect.h = longest_side * 1.5;
    sprite->radius = longest_side / 2;
    sprite->tex_radius = longest_side * 0.75;

    // Create a new texture that the sprite can be rotated and rendered to.
    sprite->text = SDL_CreateTexture(ast->rend,
                                    SDL_PIXELFORMAT_RGBA8888,
                                    SDL_TEXTUREACCESS_TARGET,
                                    sprite->rect.w,
                                    sprite->rect.h);
    if (!sprite->text)
        return ERROR_TEXT;

    // Make the texture transparent so only the player is seen.
    if (SDL_SetTextureBlendMode(sprite->text, SDL_BLENDMODE_BLEND))
        return ERROR_BLND;
    // Center the orignal rects to the center of the new rect.
    sprite->orig_rect.x = (sprite->rect.w - sprite->orig_rect.w) / 2;
    sprite->orig_rect.y = (sprite->rect.h - sprite->orig_rect.h) / 2;
    return 0;
}

enum Errors sprite_update(SDL_Renderer *rend, struct Sprite *sprite) {
    sprite->x += sprite->x_vel;
    sprite->y += sprite->y_vel;
    offscreen_check(sprite);
    sprite->rect.x = (int)sprite->x;
    sprite->rect.y = (int)sprite->y;
    if (sprite->update) {
        sprite->angle += sprite->spin;
        enum Errors rvalue = sprite_rotate(sprite, rend);
        if (rvalue)
            return rvalue;
    }
    return 0;
}

enum Errors sprite_rotate(struct Sprite *sprite, SDL_Renderer *rend) {
    // Keep the player.angle within -360 to 360.
    if (sprite->angle > 360)
        sprite->angle -= 360;
    else if (sprite->angle < 0)
        sprite->angle += 360;

    // Set the target texture as renderer.
    if (SDL_SetRenderTarget( rend, sprite->text ))
        return ERROR_TARG;

    // Clear the texture.
    if (SDL_RenderClear(rend))
        return ERROR_CLR;

    // Render original sprite texture to the target texture with an angle.
    if (SDL_RenderCopyEx(rend, sprite->orig_text, NULL, &sprite->orig_rect, sprite->angle, NULL, SDL_FLIP_NONE ))
        return ERROR_XCPY;

    // Set the renderer back to the back buffer.
    if (SDL_SetRenderTarget( rend, NULL ))
        return ERROR_TARG;

    return 0;
}

float pos_get(struct Sprite *sprite, enum Positions pos) {
    float value = 0;
    if (pos == left_x) {
        value = sprite->x + sprite->tex_radius - sprite->radius;
    } else if (pos == right_x) {
        value = sprite->x + sprite->tex_radius + sprite->radius;
    } else if (pos == center_x) {
        value = sprite->x + sprite->tex_radius;
    } else if (pos == top_y) {
        value = sprite->y + sprite->tex_radius - sprite->radius;
    } else if (pos == bottom_y) {
        value = sprite->y + sprite->tex_radius + sprite->radius;
    } else if (pos == center_y) {
        value = sprite->y + sprite->tex_radius;
    }
    return value;
}

void pos_set(struct Sprite *sprite, enum Positions pos, float new_pos) {
    if (pos == left_x) {
        sprite->x = new_pos - sprite->tex_radius + sprite->radius;
    } else if (pos == right_x) {
        sprite->x = new_pos - sprite->tex_radius - sprite->radius;
    } else if (pos == center_x) {
        sprite->x = new_pos - sprite->tex_radius;
    } else if (pos == top_y) {
        sprite->y = new_pos - sprite->tex_radius + sprite->radius;
    } else if (pos == bottom_y) {
        sprite->y = new_pos - sprite->tex_radius - sprite->radius;
    } else if (pos == center_y) {
        sprite->y = new_pos - sprite->tex_radius;
    }
}

// Check if the sprite has gone off screen, move to opposite size.
void offscreen_check(struct Sprite *sprite) {
    if (pos_get(sprite, right_x) < 0) {
        pos_set(sprite, left_x, WIDTH);
        pos_set(sprite, center_y, HEIGHT - pos_get(sprite, center_y));
    } else if (pos_get(sprite, left_x) > WIDTH ) {
        pos_set(sprite, right_x, 0);
        pos_set(sprite, center_y, HEIGHT - pos_get(sprite, center_y));
    }
    if (pos_get(sprite, bottom_y) < 0) {
        pos_set(sprite, top_y, HEIGHT);
        pos_set(sprite, center_x, WIDTH - pos_get(sprite, center_x));
    } else if (pos_get(sprite, top_y) > HEIGHT ) {
        pos_set(sprite, bottom_y, 0);
        pos_set(sprite, center_x, WIDTH - pos_get(sprite, center_x));
    }
}

bool collision_check(struct Sprite *sprite1, struct Sprite *sprite2) {
    short dist = 0;
    dist = hypot((pos_get(sprite1, center_x) - pos_get(sprite2, center_x)), \
            (pos_get(sprite1, center_y) - pos_get(sprite2, center_y)));
    if (dist < (sprite1->radius + sprite2->radius))
        return true;
    else
        return false;
}
