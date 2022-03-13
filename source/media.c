#include "media.h"

 // Loads image by name and returns texture.
enum Errors texts_load(const char* SPRITE_NAMES[], unsigned short namec, struct Game *ast) {
    if (namec != ast->textc)
        return ERROR_ARRY;

    // load all the textures into a textures array.
    for (unsigned short i = 0; i < namec; i++) {
        // create the full image and path name, load image, convert to texture.
        char full_name[50] = "";
        snprintf(full_name, LEN(full_name), "%s%s%s%s", IMGFOLD, "/", SPRITE_NAMES[i], ".png");

        SDL_Surface* surface = IMG_Load(full_name);
        if (!surface)
            return ERROR_SURF;

        ast->textv[i] = SDL_CreateTextureFromSurface(ast->rend, surface);
        if (!ast->textv[i])
            return ERROR_TEXT;

        SDL_FreeSurface(surface);
    }

    if (SDL_QueryTexture(ast->textv[icon], NULL, NULL, &ast->icon_rect.w, &ast->icon_rect.h))
        return ERROR_QTEX;
    return 0;
}

enum Errors audio_load(struct Game *ast) {
    ast->music = Mix_LoadMUS( "music/falling.ogg" );
    if(!ast->music)
        return ERROR_MUSC;

    ast->laser = Mix_LoadWAV("sounds/laser.ogg");
    if(!ast->laser)
        return ERROR_SND;

    ast->boom = Mix_LoadWAV("sounds/boom.ogg");
    if(!ast->boom)
        return ERROR_SND;

    ast->collide = Mix_LoadWAV("sounds/collide.ogg");
    if(!ast->collide)
        return ERROR_SND;

    ast->thrusters = Mix_LoadWAV("sounds/thrusters.ogg");
    if(!ast->thrusters)
        return ERROR_SND;

    return 0;
}
