#include "exit.h"

// Release memory and null pointers before exiting.
void memory_release_exit(struct Game *ast, struct Player *player, struct Shots *shots, struct Asteroids *asteroids, struct Messages *msg) {
    switch (ast->exit_status) {
        case ERROR_SDL:
            fprintf(stderr, "Error initializing SDL: %s\n", SDL_GetError());
            break;
        case ERROR_WIN:
            fprintf(stderr, "Error creating window: %s\n", SDL_GetError());
            break;
        case ERROR_REND:
            fprintf(stderr, "Error creating renderer: %s\n", SDL_GetError());
            break;
        case ERROR_TTF:
            fprintf(stderr, "Error initializing SDL_ttf: %s\n", TTF_GetError());
            break;
        case ERROR_IMG:
            fprintf(stderr, "Error initializing SDL_image: %s\n", IMG_GetError());
            break;
        case ERROR_NIMG:
            fprintf(stderr, "Expected loaded image missing!\n");
            break;
        case ERROR_ICON:
            fprintf(stderr, "Error creating icon surface: %s\n", SDL_GetError());
            break;
        case ERROR_BOND:
            fprintf(stderr, "Populating rectv array went out of bounds!\n");
            break;
        case ERROR_FONT:
            fprintf(stderr, "Error creating font: %s\n", TTF_GetError());
            break;
        case ERROR_SURF:
            fprintf(stderr, "Error creating a surface: %s\n", SDL_GetError());
            break;
        case ERROR_TEXT:
            fprintf(stderr, "Error creating a texture: %s\n", SDL_GetError());
            break;
        case ERROR_ARRY:
            fprintf(stderr, "Error an array was not the expected length:\n");
            break;
        case ERROR_MUSC:
            fprintf(stderr, "Failed to load music: %s\n", Mix_GetError());
            break;
        case ERROR_SND:
            fprintf(stderr, "Failed to load sound effect: %s\n", Mix_GetError());
            break;
        case ERROR_MIX:
            fprintf(stderr, "Error initializing SDL_mixer: %s\n", Mix_GetError());
            break;
        case ERROR_MALC:
            fprintf(stderr, "Failed to malloc new asteroid!\n");
            break;
        case ERROR_TARG:
            fprintf(stderr, "Error setting renderer target: %s\n", SDL_GetError());
            break;
        case ERROR_COPY:
            fprintf(stderr, "Error while rendering texture: %s\n", SDL_GetError());
            break;
        case ERROR_XCPY:
            fprintf(stderr, "Error while rotating texture: %s\n", SDL_GetError());
            break;
        case ERROR_CLR:
            fprintf(stderr, "Error while clearing renderer: %s\n", SDL_GetError());
            break;
        case ERROR_QTEX:
            fprintf(stderr, "Error while querying texture: %s\n", SDL_GetError());
            break;
        case ERROR_BLND:
            fprintf(stderr, "Error setting texture blend mode: %s\n", SDL_GetError());
            break;
        case ERROR_PRES:
            fprintf(stderr, "Error during Render Present: %s\n", SDL_GetError());
            break;
        default:
            break;
    }

    // Free music and sounds chunks. Close Audio.
    Mix_FreeMusic(ast->music);
    ast->music = NULL;
    Mix_FreeChunk(ast->laser);
    ast->laser = NULL;
    Mix_FreeChunk(ast->boom);
    ast->boom = NULL;
    Mix_FreeChunk(ast->collide);
    ast->collide = NULL;
    Mix_FreeChunk(ast->thrusters);
    ast->thrusters = NULL;
    Mix_Quit();

    // Close fonts.
    TTF_CloseFont(msg->title.font);
    TTF_CloseFont(msg->score.font);
    TTF_Quit();

    // Free Messages textures.
    SDL_DestroyTexture(msg->title.text);
    msg->title.text = NULL;
    SDL_DestroyTexture(msg->score.text);
    msg->score.text = NULL;

    // Free main game textures.
    for (unsigned short i = 0; i < ast->textc; i++) {
        if (ast->textv[i]) {
            SDL_DestroyTexture(ast->textv[i]);
            ast->textv[i] = NULL;
        }
    }

    // free player sprite texture.
    for (unsigned short i = 0; i < LEN(player->orig_text); i++) {
        SDL_DestroyTexture(player->orig_text[i]);
        player->orig_text[i] = NULL;
    }
    SDL_DestroyTexture(player->sprite.text);
    player->sprite.text = NULL;

    // Free textures for each shot sprite.
    for (unsigned short i = 0; i < shots->shotc; i++) {
        if (shots->shotv[i].sprite.text) {
            SDL_DestroyTexture(shots->shotv[i].sprite.text);
            shots->shotv[i].sprite.text = NULL;
        }
    }

    asteroid_delete_all(asteroids);

    IMG_Quit();

    // Destroy window and render.
    SDL_DestroyRenderer(ast->rend);
    ast->rend = NULL;
    SDL_DestroyWindow(ast->win);
    ast->win = NULL;

    SDL_Quit();
    exit(ast->exit_status);
}
