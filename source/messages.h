#ifndef MESSAGE_H
#define MESSAGE_H

#include "common.h"

struct Message {
    SDL_Color color;
    TTF_Font *font;
    SDL_Texture* text;
    SDL_Rect rect;
};

struct Messages {
    struct Message title;
    struct Message score;
};

enum Errors message_create(const char* new_text, enum MsgTypes msgt, struct Message *message, SDL_Renderer *rend);
enum Errors message_update(const char * string, const long num, enum MsgTypes msgt, struct Messages *msg, SDL_Renderer *rend);
enum Errors messages_reset(struct Messages *msg, SDL_Renderer *rend);
enum Errors messages_load(struct Messages *msg, SDL_Renderer *rend);

#endif
