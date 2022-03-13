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

if [ ! -f "astroids-sdl" ]; then
    echo
    echo "astroids-sdl file not found."
    echo "Try running the make command to build astroids-sdl."
    echo
    exit 1
fi

mkdir -p /usr/share/astroids
cp -r images /usr/share/astroids
cp -r sounds /usr/share/astroids
cp -r music /usr/share/astroids

cp freesansbold.ttf /usr/share/astroids
cp astroids /usr/local/bin
chmod +x /usr/local/bin/astroids
cp astroids-sdl /usr/share/astroids
cp astroids-sdl.desktop /usr/share/applications
