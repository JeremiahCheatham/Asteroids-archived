#!/usr/bin/env bash
if [ 0 != $UID ]; then
    echo "This script must be run as root with sudo.";
    exit 1;
fi

mkdir -p /usr/share/astroids
cp -r images /usr/share/astroids
cp -r sounds /usr/share/astroids
cp -r music /usr/share/astroids

cp astroids-pygame.py /usr/share/astroids
chmod +x /usr/share/astroids/astroids-pygame.py
cp astroids-pygame /usr/local/bin
chmod +x /usr/local/bin/astroids-pygame
cp astroids-pygame.desktop /usr/share/applications
