CC ?= gcc
CCFLAGS ?= `sdl2-config --cflags` -ggdb3 -O0 --std=c99 -Wall -Wextra -Wwrite-strings
LDFLAGS := `sdl2-config --libs` -lm -lSDL2_image -lSDL2_ttf -lSDL2_mixer
TESTFLAGS := -fsanitize=leak -fsanitize=address -fsanitize=undefined
TARGET := asteroids-sdl
SRCFOLD := source
SOURCES := $(SRCFOLD)/$(TARGET).c $(SRCFOLD)/fps.c $(SRCFOLD)/media.c $(SRCFOLD)/player.c $(SRCFOLD)/sprites.c $(SRCFOLD)/sdl_init.c $(SRCFOLD)/shots.c $(SRCFOLD)/asteroids.c $(SRCFOLD)/exit.c $(SRCFOLD)/messages.c $(SRCFOLD)/game_mode.c

$(TARGET):
	$(CC) $(LDFLAGS) $(CCFLAGS) $(SOURCES) -o $(TARGET)

clean:
	rm -rf $(TARGET)

rebuild:
	$(clean)
	$(CC) $(LDFLAGS) $(CCFLAGS) $(SOURCES) -o $(TARGET)

test:
	$(clean)
	$(CC) $(LDFLAGS) $(CCFLAGS) $(TESTFLAGS) $(SOURCES) -o $(TARGET)
