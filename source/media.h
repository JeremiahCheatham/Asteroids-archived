#ifndef MEDIA_H
#define MEDIA_H

#include "common.h"

enum Errors texts_load(const char* SPRITE_NAMES[], unsigned short namec, struct Game *ast);
enum Errors audio_load(struct Game *ast);

#endif
