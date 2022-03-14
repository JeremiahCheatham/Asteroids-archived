// Included header files.
#include "common.h"
#include "media.h"
#include "fps.h"
#include "player.h"
#include "sprites.h"
#include "sdl_init.h"
#include "shots.h"
#include "astroids.h"
#include "exit.h"
#include "messages.h"
#include "game_mode.h"

const char* SPRITE_NAMES[] = { XMACRO(TO_STR) };

int main() {
    SDL_Event event;
    const Uint8 *keystate = SDL_GetKeyboardState(NULL);

    struct Game ast = {
        .win = NULL,
        .rend = NULL,
        .music = NULL,
        .laser = NULL,
        .boom = NULL,
        .collide = NULL,
        .thrusters = NULL,
        .textc = LEN(ast.textv),
        .frame_delay = 1000.0f / FPS,
        .show_fps = false,
        .level = LEVEL,
        .lives = LIVES,
        .exit_status = 0,
        .mode = MODE_START
    };

    struct Astroids astroids = { .first_astroid = NULL };

    struct Player player = {
        .sprite.text = NULL,
        .orig_text[0] = NULL
    };

    struct Shots shots = {
        .shotv[0].sprite.text = NULL,
        .shotc = LEN(shots.shotv),
        .shots_count = 100
    };

    struct Messages msg = {
        .score.font = NULL,
        .score.text = NULL,
        .title.font = NULL,
        .title.text = NULL
    };

    // Initialize SDL, create window and renderer.
    ast.exit_status = sdl_setup(&ast);
    if (ast.exit_status)
        memory_release_exit(&ast, &player, &shots, &astroids, &msg);

    // load all the textures into a textures array.
    ast.exit_status = texts_load(SPRITE_NAMES, LEN(SPRITE_NAMES), &ast);
    if (ast.exit_status)
        memory_release_exit(&ast, &player, &shots, &astroids, &msg);

    // load score and title messages.
    ast.exit_status = messages_load(&msg, ast.rend);
    if (ast.exit_status)
        memory_release_exit(&ast, &player, &shots, &astroids, &msg);

    // load all audio.
    ast.exit_status = audio_load(&ast);
    if (ast.exit_status)
        memory_release_exit(&ast, &player, &shots, &astroids, &msg);

    // Create main player from sprite struct.
    ast.exit_status = player_create(&player, &ast);
    if (ast.exit_status)
        memory_release_exit(&ast, &player, &shots, &astroids, &msg);

    //Play the music
    Mix_PlayMusic(ast.music, -1);

    ast.exit_status = game_mode(&ast, &player, &shots, &astroids, &msg);
    if (ast.exit_status)
        memory_release_exit(&ast, &player, &shots, &astroids, &msg);

    while (!ast.exit_status) {
        // process events

        // Check key events, key pressed or released.
        while (SDL_PollEvent(&event)) {
            switch (event.type) {

                case SDL_QUIT:
                    // handling of close button
                    ast.exit_status = QUIT;
                    break;

                case SDL_USEREVENT:
                    if (event.user.code == TIMER_IMMUNE) {
                        player_immunity_toggle(&player, false);
                    } else if (event.user.code == TIMER_GAME) {
                        game_mode(&ast, &player, &shots, &astroids, &msg);
                    }
                    break;

                case SDL_KEYDOWN:
                    // keyboard API for key pressed
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_ESCAPE:
                            ast.exit_status = QUIT;
                            break;
                        case SDL_SCANCODE_SPACE:
                            if (ast.mode == MODE_PLAY)
                                shot_create(&shots, &player.sprite, &ast);
                            break;
                        case SDL_SCANCODE_M:
                            if(Mix_PausedMusic())
                                Mix_ResumeMusic();
                            else
                                Mix_PauseMusic();
                            break;
                        default:
                            break;
                    }
            }
        }
        if (ast.exit_status)
            continue;
        if (ast.mode == MODE_PLAY) {
            // Check current state of keys, not key event.
            if (keystate[SDL_SCANCODE_W] || keystate[SDL_SCANCODE_UP] ) {
                player_thrust_update(&player.sprite);
                if (!player.engine) {
                    player_engine_toggle(&player, &ast);
                }
            } else {
                if (player.engine) {
                    player_engine_toggle(&player, &ast);
                }
            }
            if (keystate[SDL_SCANCODE_A] || keystate[SDL_SCANCODE_LEFT] ) {
                player.sprite.angle -= 5;
                player.sprite.update = true;
            }
            if (keystate[SDL_SCANCODE_D] || keystate[SDL_SCANCODE_RIGHT] ) {
                player.sprite.angle += 5;
                player.sprite.update = true;
            }

            ast.exit_status = player_update(ast.rend, &player);
            if (ast.exit_status)
                continue;

            shots_update(&shots);

            ast.exit_status = astroids_update(ast.rend, &astroids);
            if (ast.exit_status)
                continue;

            ast.exit_status = astroid_collision_check(&ast, &player, &shots, &astroids, &msg);
            if (ast.exit_status)
                continue;

        }
        // Clear the existing renderer.
        if (SDL_RenderClear(ast.rend)) {
            ast.exit_status = ERROR_CLR;
            continue;
        }

        // Draw the images to the renderer.
        if ( SDL_RenderCopy(ast.rend, ast.textv[background], NULL, NULL)) {
            ast.exit_status = ERROR_COPY;
            continue;
        }

        if (ast.mode == MODE_PLAY) {
            ast.exit_status = astroids_render(ast.rend, &astroids);
            if (ast.exit_status)
                continue;

            ast.exit_status = player_render(ast.rend, &player);
            if (ast.exit_status)
                continue;

            ast.exit_status = shots_render(ast.rend, &shots);
            if (ast.exit_status)
                continue;

            ast.icon_rect.y = 10;
            for (int i = 1; i <= ast.lives; i++) {
                ast.icon_rect.x = 30 * i;
                if (SDL_RenderCopy(ast.rend, ast.textv[icon], NULL, &ast.icon_rect)) {
                    ast.exit_status = ERROR_COPY;
                    break;
                }
            }
            if (ast.exit_status)
                continue;

            // Draw the score to the renderer.
            if ( SDL_RenderCopy(ast.rend, msg.score.text, NULL, &msg.score.rect)) {
                ast.exit_status = ERROR_COPY;
                continue;
            }
        } else {
            // Draw the title to the renderer.
            if ( SDL_RenderCopy(ast.rend, msg.title.text, NULL, &msg.title.rect)) {
                ast.exit_status = ERROR_COPY;
                continue;
            }
        }

        // Swap the back buffer to the front.
        SDL_RenderPresent(ast.rend);

        // Print FPS to standard output.
        if (ast.show_fps)
            fps_print();

        // Calculate delay needed for the FPS.
        fps_delay(&ast);
    }

    // Release memory and null pointers before exiting.
    memory_release_exit(&ast, &player, &shots, &astroids, &msg);
}
