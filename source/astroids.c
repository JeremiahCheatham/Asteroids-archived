#include "astroids.h"
#include "game_mode.h"

enum Errors astroid_populate(struct Astroids *astroids, struct Game *ast) {
    astroid_delete_all(astroids);
    srand(time(NULL));
    unsigned short CENTERX = WIDTH / 2;
    unsigned short CENTERY = HEIGHT / 2;
    for(short unsigned i = 0; i < ast->level; i++) {
        float angle = (rand() % 360) - 180;
        float spin = ((rand() % 200) - 100) / 100.0f;
        float speed = ((rand() % 20) - 10) / 10.0f;

        enum Errors rvalue = astroid_create(ast, astroids, astroidlarge, angle, spin, speed);
        if (rvalue)
            return rvalue;

        short dist = 0;
        short new_x = 0;
        short new_y = 0;
        do {
            new_x = rand() % WIDTH;
            new_y = rand() % HEIGHT;
            dist = hypot((new_x - CENTERX), (new_y - CENTERY));
        } while (dist < 200);
        pos_set(&astroids->first_astroid->sprite, center_x, new_x);
        pos_set(&astroids->first_astroid->sprite, center_y, new_y);
        astroids->first_astroid->sprite.rect.x = (int)astroids->first_astroid->sprite.x;
        astroids->first_astroid->sprite.rect.y = (int)astroids->first_astroid->sprite.y;
    }
    return 0;
}

enum Errors astroid_create(struct Game *ast, struct Astroids *astroids, enum Sprites size, short angle, float spin, float speed) {
    struct Astroid *new_astroid = malloc (sizeof(struct Astroid));
    if (!new_astroid)
        return ERROR_MALC;

    new_astroid->previous = NULL;
    new_astroid->next = NULL;
    new_astroid->size = size;

    enum Errors rvalue = sprite_create(&new_astroid->sprite, size, ast);
    if (rvalue)
        return rvalue;

    new_astroid->sprite.spin = spin;

    // Set shot angle to player angle and rotate shot to new angle.
    new_astroid->sprite.update = true;
    new_astroid->sprite.angle = angle;

    rvalue = sprite_rotate(&new_astroid->sprite, ast->rend);
    if (rvalue)
        return rvalue;

    // Set the x and y velocity based on player angle.
    float radian = angle * PI / 180;
    new_astroid->sprite.y_vel = cos(radian) * -speed;
    new_astroid->sprite.x_vel = sin(radian) * speed;
    if (astroids->first_astroid) {
        new_astroid->next = astroids->first_astroid;
        astroids->first_astroid->previous = new_astroid;
    }
    astroids->first_astroid = new_astroid;
    return 0;
}

void astroid_delete(struct Astroid *delete_astroid, struct Astroids *astroids) {
    if (delete_astroid->next && delete_astroid->previous) {
        delete_astroid->previous->next = delete_astroid->next;
        delete_astroid->next->previous = delete_astroid->previous;
    } else if (delete_astroid->next) {
        delete_astroid->next->previous = NULL;
        astroids->first_astroid = delete_astroid->next;
    } else if (delete_astroid->previous) {
        delete_astroid->previous->next = NULL;
    } else {
        astroids->first_astroid = NULL;
    }
    SDL_DestroyTexture(delete_astroid->sprite.text);
    delete_astroid->sprite.text = NULL;
    free(delete_astroid);
}

void astroid_delete_all(struct Astroids *astroids) {
    struct Astroid *astroid = astroids->first_astroid;
    while (astroid) {
        astroid_delete(astroid, astroids);
        astroid = astroids->first_astroid;
    }
}

enum Errors astroids_update(SDL_Renderer *rend, struct Astroids *astroids) {
    struct Astroid *current;
    for(current = astroids->first_astroid; current != NULL; current = current->next) {
        enum Errors rvalue = sprite_update(rend, &current->sprite);
        if (rvalue)
            return rvalue;
    }
    return 0;
}

enum Errors astroid_collision_check(struct Game *ast, struct Player *player, struct Shots *shots, struct Astroids *astroids, struct Messages *msg) {
    struct Astroid *current;
    enum Errors rvalue = 0;
    for (unsigned short i = 0; i < shots->shotc; i++) {
        if (shots->shotv[i].sprite.text) {
            current = astroids->first_astroid;
            while (current) {
                if (collision_check(&current->sprite, &shots->shotv[i].sprite)) {
                    Mix_PlayChannel(-1, ast->boom, 0);
                    if (current->size == astroidlarge || current->size == astroidmedium) {
                        rvalue = astroid_split(ast, astroids, current, shots->shotv[i].sprite.angle);
                        if (rvalue)
                            return rvalue;
                    }
                    ast->score += shots->shotv[i].shot_count * ast->level;

                    rvalue = message_update("SCORE:", ast->score, MSG_SCORE, msg, ast->rend);
                    if (rvalue)
                        return rvalue;

                    struct Astroid *delete_astroid = current;
                    current = current->next;
                    SDL_DestroyTexture(shots->shotv[i].sprite.text);
                    shots->shotv[i].sprite.text = NULL;
                    astroid_delete(delete_astroid, astroids);
                    if (!astroids->first_astroid) {
                        ast->mode = MODE_INTRO;
                        ast->level += 1;
                        ast->lives += 1;
                        rvalue = game_mode(ast, player, shots, astroids, msg);
                        if (rvalue)
                            return rvalue;
                    }
                } else {
                    current = current->next;
                }
            }
        }
    }
    if (!player->immunity) {
        for(current = astroids->first_astroid; current != NULL; current = current->next) {
            if (collision_check(&current->sprite, &player->sprite)) {
                Mix_PlayChannel(-1, ast->collide, 0);
                player_immunity_toggle(player, true);
                if (ast->lives) {
                    ast->lives -=1;
                } else {
                    rvalue = game_mode(ast, player, shots, astroids, msg);
                    if (rvalue)
                        return rvalue;
                }
                break;
            }
        }
    }
    return 0;
}


// Split astroid in 2
enum Errors astroid_split(struct Game *ast, struct Astroids *astroids, struct Astroid *astroid, float angle) {
    float spin = ((rand() % 200) - 100) / 25.0f;
    float speed = (rand() % 20) / 10.0f;
    unsigned short new_size = astroid->size - 1;

    enum Errors rvalue = astroid_create(ast, astroids, new_size, angle + 90, spin, speed);
    if (rvalue)
        return rvalue;

    pos_set(&astroids->first_astroid->sprite, center_x, pos_get(&astroid->sprite, center_x));
    pos_set(&astroids->first_astroid->sprite, center_y, pos_get(&astroid->sprite, center_y));
    astroids->first_astroid->sprite.rect.x = (int)astroids->first_astroid->sprite.x;
    astroids->first_astroid->sprite.rect.y = (int)astroids->first_astroid->sprite.y;

    rvalue = astroid_create(ast, astroids, new_size, angle + 270, -spin, speed);
    if (rvalue)
        return rvalue;

    pos_set(&astroids->first_astroid->sprite, center_x, pos_get(&astroid->sprite, center_x));
    pos_set(&astroids->first_astroid->sprite, center_y, pos_get(&astroid->sprite, center_y));
    astroids->first_astroid->sprite.rect.x = (int)astroids->first_astroid->sprite.x;
    astroids->first_astroid->sprite.rect.y = (int)astroids->first_astroid->sprite.y;
    return 0;
}

enum Errors astroids_render(SDL_Renderer *rend, struct Astroids *astroids) {
    struct Astroid *current;
    for(current = astroids->first_astroid; current != NULL; current = current->next) {
        if (SDL_RenderCopy(rend, current->sprite.text, NULL, &current->sprite.rect))
            return ERROR_COPY;
    }
    return 0;
}
