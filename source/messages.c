#include "messages.h"


enum Errors message_create(const char* new_text, enum MsgTypes msgt, struct Message *message, SDL_Renderer *rend) {
    SDL_Surface *surf = TTF_RenderText_Blended(message->font, new_text, message->color);
    if (!surf)
        return ERROR_SURF;

    message->text = SDL_CreateTextureFromSurface(rend, surf);
    if (!message->text)
        return ERROR_TEXT;

    // Get a rect from the original image.
    if (SDL_QueryTexture(message->text, NULL, NULL, &message->rect.w, &message->rect.h))
        return ERROR_QTEX;

    if (msgt == MSG_TITLE) {
        message->rect.x = (WIDTH / 2) - (message->rect.w / 2);
        message->rect.y = (HEIGHT / 2) - (message->rect.h / 2);
    } else if (msgt == MSG_SCORE) {
        message->rect.x = (WIDTH - message->rect.w) - 30;
        message->rect.y = 10;
    }
    return 0;
}

enum Errors message_update(const char * string, const long num, enum MsgTypes msgt, struct Messages *msg, SDL_Renderer *rend) {
    if (num < 0 ) {
        enum Errors rvalue = message_create(string, msgt, msgt ? &msg->score : &msg->title, rend);
        if (rvalue)
            return rvalue;
    } else {
        unsigned num_length = 0;
        if (num == 0 ) {
            num_length = 1;
        } else {
            unsigned n = num;
            while(n) {
                n /= 10;
                num_length++;
            }
        }
        unsigned new_length = num_length + strlen(string) + 2;
        char new_string[new_length];
        snprintf(new_string, new_length, "%s %ld", string, num);
        enum Errors rvalue = message_create(new_string, msgt, msgt ? &msg->score : &msg->title, rend);
        if (rvalue)
            return rvalue;
    }
    return 0;
}

enum Errors messages_reset(struct Messages *msg, SDL_Renderer *rend) {
    // load score and title messages.
    enum Errors rvalue = message_update("ASTROIDS!", -1, MSG_TITLE, msg, rend);
    if (rvalue)
        return rvalue;

    rvalue = message_update("SCORE:", 0, MSG_SCORE, msg, rend);
    if (rvalue)
        return rvalue;

    return 0;
}

enum Errors messages_load(struct Messages *msg, SDL_Renderer *rend) {
    SDL_Color color = {255, 255, 255, 255};
    msg->title.color = color;
    msg->score.color = color;
    msg->title.font = TTF_OpenFont(FONT_NAME, TITLE_FONT_SIZE);
    msg->score.font = TTF_OpenFont(FONT_NAME, SCORE_FONT_SIZE);
    if (!msg->title.font || !msg->score.font)
        return ERROR_TTF;

    enum Errors rvalue = messages_reset(msg, rend);
    if (rvalue)
        return rvalue;

    return 0;
}
