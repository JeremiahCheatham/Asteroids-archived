.POSIX:
CC      = gcc
CFLAGS  = -std=c99 -ggdb3 -Wall -Wextra -Wwrite-strings
LDFLAGS =
LDLIBS  = `sdl2-config --libs` -lm -lSDL2_image -lSDL2_ttf -lSDL2_mixer
all: astroids-sdl
source/asteroids.o: source/asteroids.c source/asteroids.h source/common.h \
 source/sprites.h source/shots.h source/player.h source/messages.h \
 source/game_mode.h
source/asteroids-sdl.o: source/asteroids-sdl.c source/common.h \
 source/media.h source/fps.h source/player.h source/sprites.h \
 source/sdl_init.h source/shots.h source/asteroids.h source/messages.h \
 source/exit.h source/game_mode.h
source/exit.o: source/exit.c source/exit.h source/common.h \
 source/sprites.h source/shots.h source/asteroids.h source/player.h \
 source/messages.h
source/fps.o: source/fps.c source/fps.h source/common.h
source/game_mode.o: source/game_mode.c source/game_mode.h source/common.h \
 source/player.h source/sprites.h source/shots.h source/asteroids.h \
 source/messages.h
source/media.o: source/media.c source/media.h source/common.h
source/messages.o: source/messages.c source/messages.h source/common.h
source/player.o: source/player.c source/player.h source/common.h \
 source/sprites.h
source/sdl_init.o: source/sdl_init.c source/sdl_init.h source/common.h
source/shots.o: source/shots.c source/shots.h source/common.h \
 source/sprites.h
source/sprites.o: source/sprites.c source/sprites.h source/common.h
obj = source/asteroids.o source/asteroids-sdl.o source/exit.o source/fps.o source/game_mode.o source/media.o source/messages.o source/player.o source/sdl_init.o source/shots.o source/sprites.o
astroids-sdl: $(obj)
	$(CC) $(LDFLAGS) -o $@ $(obj) $(LDLIBS)
clean:
	rm -f astroids-sdl $(obj)
.c.o:
	$(CC) `sdl2-config --cflags` -c $(CFLAGS) -o $@ $<
