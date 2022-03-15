#!/usr/bin/env bash
if [ 0 != $UID ]; then
    echo "This script must be run as root with sudo.";
    exit 1;
fi

mkdir -p /usr/share/asteroids
cp -r images /usr/share/asteroids
cp -r sounds /usr/share/asteroids
cp -r music /usr/share/asteroids

cp asteroids-pygame.py /usr/share/asteroids
chmod +x /usr/share/asteroids/asteroids-pygame.py
cp asteroids-pygame /usr/local/bin
chmod +x /usr/local/bin/asteroids-pygame
cp asteroids-pygame.desktop /usr/share/applications
