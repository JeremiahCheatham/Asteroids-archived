#include "game_mode.h"

enum Errors game_mode(struct Game *ast, struct Player *player, struct Shots *shots, struct Astroids *astroids, struct Messages *msg) {
    enum Errors rvalue = 0;
    if (player->engine)
        player_engine_toggle(player, ast);
    switch (ast->mode) {
        case MODE_SCORE:
        case MODE_START:
            ast->mode = MODE_INTRO;
            ast->level = LEVEL;
            ast->lives = LIVES;
            ast->score = 0;

            rvalue = messages_reset(msg, ast->rend);
            if (rvalue)
                return rvalue;

            ast->mode_timer = SDL_AddTimer(5000, game_mode_timer, NULL);
            break;
        case MODE_INTRO:
            ast->mode = MODE_LEVEL;

            rvalue = message_update("LEVEL:", ast->level, MSG_TITLE, msg, ast->rend);
            if (rvalue)
                return rvalue;

            ast->mode_timer = SDL_AddTimer(5000, game_mode_timer, NULL);
            break;
        case MODE_LEVEL:
            ast->mode = MODE_PLAY;
            shots->shots_count = 100;
            shots_delete_all(shots);

            rvalue = astroid_populate(astroids, ast);
            if (rvalue)
                return rvalue;

            rvalue = player_reset(player, ast);
            if (rvalue)
                return rvalue;

            break;
        case MODE_PLAY:
            ast->mode = MODE_OVER;

            rvalue = message_update("GAME OVER!", -1, MSG_TITLE, msg, ast->rend);
            if (rvalue)
                return rvalue;

            ast->level += 1;
            ast->mode_timer = SDL_AddTimer(5000, game_mode_timer, NULL);
            break;
        case MODE_OVER:
            ast->mode = MODE_SCORE;

            rvalue = message_update("SCORE:", ast->score, MSG_TITLE, msg, ast->rend);
            if (rvalue)
                return rvalue;

            ast->mode_timer = SDL_AddTimer(5000, game_mode_timer, NULL);
            break;
        default:
            break;
    }
    return 0;
}

// Create a callback event for immunity.
Uint32 game_mode_timer() {
    SDL_Event event;
    event.type = SDL_USEREVENT;
    event.user.code = TIMER_GAME;
    event.user.data1 = NULL;
    event.user.data2 = NULL;
    SDL_PushEvent(&event);
    return 0;
}
