#!/usr/bin/env bash
if [ 0 != $UID ]; then
    echo
    echo "This script must be run as root with sudo."
    echo
    exit 1
fi

if ! make rebuild; then
    echo
    echo "Well that didn't work."
    echo "To compile you need a basic biuld enviroment."
    echo "You will also need SDL2 installed."
    echo
    exit 1
fi

if [ ! -f "asteroids-sdl" ]; then
    echo
    echo "asteroids-sdl file not found."
    echo "Try running the make command to build asteroids-sdl."
    echo
    exit 1
fi

mkdir -p /usr/share/asteroids
cp -r images /usr/share/asteroids
cp -r sounds /usr/share/asteroids
cp -r music /usr/share/asteroids

cp freesansbold.ttf /usr/share/asteroids
cp asteroids /usr/local/bin
chmod +x /usr/local/bin/asteroids
cp asteroids-sdl /usr/share/asteroids
cp asteroids-sdl.desktop /usr/share/applications
