#ifndef COMMON_H
#define COMMON_H

// Included header files.
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>

// Define directives for constants.
#define MY_SDL_FLAGS (SDL_INIT_VIDEO|SDL_INIT_TIMER|SDL_INIT_AUDIO)
#define TITLE "ASTEROIDS! - SDL"
#define ICON "images/icon.png"
#define WIDTH 800
#define HEIGHT 600
#define FPS 60
#define PI 3.14159265
#define THRUST 0.1
#define LIVES 1
#define LEVEL 1
#define SHIP_RADIUS 25
#define SHOT_RADIUS 5
#define SHOT_VELOCITY 10
#define SHOT_LIFE 40
#define SHOT_MAX_COUNT 5
#define IMGFOLD "images"
#define FONT_NAME "freesansbold.ttf"
#define FONT_COLOR {20, 20, 20, 255}
#define TITLE_FONT_SIZE 90
#define SCORE_FONT_SIZE 20

// Length of array macro.
#define LEN(x) (sizeof(x)/sizeof(x[0]))

// Create enum Sprite and char* SPRITE_NAMES[] with same list.
#define XMACRO(X) \
   X(ship) \
   X(ship2) \
   X(background) \
   X(icon) \
   X(shot) \
   X(asteroidsmall) \
   X(asteroidmedium) \
   X(asteroidlarge)

#define TO_ENUM(NAME) NAME,
#define TO_STR(NAME)  #NAME,

enum Sprites { XMACRO(TO_ENUM) SPRITE_LENGTH};
enum Positions { left_x, right_x, center_x, top_y, bottom_y, center_y };
enum Errors {QUIT=1, ERROR_SDL, ERROR_WIN, ERROR_REND, ERROR_TTF, ERROR_IMG, \
            ERROR_NIMG, ERROR_ICON, ERROR_BOND, ERROR_FONT, ERROR_SURF, \
            ERROR_TEXT, ERROR_ARRY, ERROR_MIX, ERROR_MUSC, ERROR_SND, \
            ERROR_MALC, ERROR_TARG, ERROR_COPY, ERROR_XCPY, ERROR_CLR, \
            ERROR_QTEX, ERROR_BLND, ERROR_PRES};
enum Modes { MODE_START, MODE_INTRO, MODE_LEVEL, MODE_PLAY, MODE_OVER, MODE_SCORE };
enum Timers {TIMER_GAME, TIMER_IMMUNE};
enum MsgTypes {MSG_TITLE, MSG_SCORE};

struct Game{
    SDL_Window *win;
    SDL_Renderer *rend;
    SDL_Texture *textv[SPRITE_LENGTH];
    SDL_Rect icon_rect;
    Mix_Music *music;
    Mix_Chunk *laser;
    Mix_Chunk *boom;
    Mix_Chunk *collide;
    Mix_Chunk *thrusters;
    unsigned short thrust_channel;
    unsigned short textc;
    float frame_delay;
    bool show_fps;
    unsigned short level;
    unsigned short lives;
    long score;
    enum Errors exit_status;
    enum Modes mode;
    SDL_TimerID mode_timer;
};

#endif
