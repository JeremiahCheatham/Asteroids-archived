#include "asteroids.h"
#include "game_mode.h"

enum Errors asteroid_populate(struct Asteroids *asteroids, struct Game *ast) {
    asteroid_delete_all(asteroids);
    srand(time(NULL));
    unsigned short CENTERX = WIDTH / 2;
    unsigned short CENTERY = HEIGHT / 2;
    for(short unsigned i = 0; i < ast->level; i++) {
        float angle = (rand() % 360) - 180;
        float spin = ((rand() % 200) - 100) / 100.0f;
        float speed = ((rand() % 20) - 10) / 10.0f;

        enum Errors rvalue = asteroid_create(ast, asteroids, asteroidlarge, angle, spin, speed);
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
        pos_set(&asteroids->first_asteroid->sprite, center_x, new_x);
        pos_set(&asteroids->first_asteroid->sprite, center_y, new_y);
        asteroids->first_asteroid->sprite.rect.x = (int)asteroids->first_asteroid->sprite.x;
        asteroids->first_asteroid->sprite.rect.y = (int)asteroids->first_asteroid->sprite.y;
    }
    return 0;
}

enum Errors asteroid_create(struct Game *ast, struct Asteroids *asteroids, enum Sprites size, short angle, float spin, float speed) {
    struct Asteroid *new_asteroid = malloc (sizeof(struct Asteroid));
    if (!new_asteroid)
        return ERROR_MALC;

    new_asteroid->previous = NULL;
    new_asteroid->next = NULL;
    new_asteroid->size = size;

    enum Errors rvalue = sprite_create(&new_asteroid->sprite, size, ast);
    if (rvalue)
        return rvalue;

    new_asteroid->sprite.spin = spin;

    // Set shot angle to player angle and rotate shot to new angle.
    new_asteroid->sprite.update = true;
    new_asteroid->sprite.angle = angle;

    rvalue = sprite_rotate(&new_asteroid->sprite, ast->rend);
    if (rvalue)
        return rvalue;

    // Set the x and y velocity based on player angle.
    float radian = angle * PI / 180;
    new_asteroid->sprite.y_vel = cos(radian) * -speed;
    new_asteroid->sprite.x_vel = sin(radian) * speed;
    if (asteroids->first_asteroid) {
        new_asteroid->next = asteroids->first_asteroid;
        asteroids->first_asteroid->previous = new_asteroid;
    }
    asteroids->first_asteroid = new_asteroid;
    return 0;
}

void asteroid_delete(struct Asteroid *delete_asteroid, struct Asteroids *asteroids) {
    if (delete_asteroid->next && delete_asteroid->previous) {
        delete_asteroid->previous->next = delete_asteroid->next;
        delete_asteroid->next->previous = delete_asteroid->previous;
    } else if (delete_asteroid->next) {
        delete_asteroid->next->previous = NULL;
        asteroids->first_asteroid = delete_asteroid->next;
    } else if (delete_asteroid->previous) {
        delete_asteroid->previous->next = NULL;
    } else {
        asteroids->first_asteroid = NULL;
    }
    SDL_DestroyTexture(delete_asteroid->sprite.text);
    delete_asteroid->sprite.text = NULL;
    free(delete_asteroid);
}

void asteroid_delete_all(struct Asteroids *asteroids) {
    struct Asteroid *asteroid = asteroids->first_asteroid;
    while (asteroid) {
        asteroid_delete(asteroid, asteroids);
        asteroid = asteroids->first_asteroid;
    }
}

enum Errors asteroids_update(SDL_Renderer *rend, struct Asteroids *asteroids) {
    struct Asteroid *current;
    for(current = asteroids->first_asteroid; current != NULL; current = current->next) {
        enum Errors rvalue = sprite_update(rend, &current->sprite);
        if (rvalue)
            return rvalue;
    }
    return 0;
}

enum Errors asteroid_collision_check(struct Game *ast, struct Player *player, struct Shots *shots, struct Asteroids *asteroids, struct Messages *msg) {
    struct Asteroid *current;
    enum Errors rvalue = 0;
    for (unsigned short i = 0; i < shots->shotc; i++) {
        if (shots->shotv[i].sprite.text) {
            current = asteroids->first_asteroid;
            while (current) {
                if (collision_check(&current->sprite, &shots->shotv[i].sprite)) {
                    Mix_PlayChannel(-1, ast->boom, 0);
                    if (current->size == asteroidlarge || current->size == asteroidmedium) {
                        rvalue = asteroid_split(ast, asteroids, current, shots->shotv[i].sprite.angle);
                        if (rvalue)
                            return rvalue;
                    }
                    ast->score += shots->shotv[i].shot_count * ast->level;

                    rvalue = message_update("SCORE:", ast->score, MSG_SCORE, msg, ast->rend);
                    if (rvalue)
                        return rvalue;

                    struct Asteroid *delete_asteroid = current;
                    current = current->next;
                    SDL_DestroyTexture(shots->shotv[i].sprite.text);
                    shots->shotv[i].sprite.text = NULL;
                    asteroid_delete(delete_asteroid, asteroids);
                    if (!asteroids->first_asteroid) {
                        ast->mode = MODE_INTRO;
                        ast->level += 1;
                        ast->lives += 1;
                        rvalue = game_mode(ast, player, shots, asteroids, msg);
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
        for(current = asteroids->first_asteroid; current != NULL; current = current->next) {
            if (collision_check(&current->sprite, &player->sprite)) {
                Mix_PlayChannel(-1, ast->collide, 0);
                player_immunity_toggle(player, true);
                if (ast->lives) {
                    ast->lives -=1;
                } else {
                    rvalue = game_mode(ast, player, shots, asteroids, msg);
                    if (rvalue)
                        return rvalue;
                }
                break;
            }
        }
    }
    return 0;
}


// Split asteroid in 2
enum Errors asteroid_split(struct Game *ast, struct Asteroids *asteroids, struct Asteroid *asteroid, float angle) {
    float spin = ((rand() % 200) - 100) / 25.0f;
    float speed = (rand() % 20) / 10.0f;
    unsigned short new_size = asteroid->size - 1;

    enum Errors rvalue = asteroid_create(ast, asteroids, new_size, angle + 90, spin, speed);
    if (rvalue)
        return rvalue;

    pos_set(&asteroids->first_asteroid->sprite, center_x, pos_get(&asteroid->sprite, center_x));
    pos_set(&asteroids->first_asteroid->sprite, center_y, pos_get(&asteroid->sprite, center_y));
    asteroids->first_asteroid->sprite.rect.x = (int)asteroids->first_asteroid->sprite.x;
    asteroids->first_asteroid->sprite.rect.y = (int)asteroids->first_asteroid->sprite.y;

    rvalue = asteroid_create(ast, asteroids, new_size, angle + 270, -spin, speed);
    if (rvalue)
        return rvalue;

    pos_set(&asteroids->first_asteroid->sprite, center_x, pos_get(&asteroid->sprite, center_x));
    pos_set(&asteroids->first_asteroid->sprite, center_y, pos_get(&asteroid->sprite, center_y));
    asteroids->first_asteroid->sprite.rect.x = (int)asteroids->first_asteroid->sprite.x;
    asteroids->first_asteroid->sprite.rect.y = (int)asteroids->first_asteroid->sprite.y;
    return 0;
}

enum Errors asteroids_render(SDL_Renderer *rend, struct Asteroids *asteroids) {
    struct Asteroid *current;
    for(current = asteroids->first_asteroid; current != NULL; current = current->next) {
        if (SDL_RenderCopy(rend, current->sprite.text, NULL, &current->sprite.rect))
            return ERROR_COPY;
    }
    return 0;
}
