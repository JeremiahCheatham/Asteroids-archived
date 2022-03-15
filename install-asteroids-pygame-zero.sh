#!/usr/bin/env bash
if [ 0 != $UID ]; then
    echo "This script must be run as root with sudo.";
    exit 1;
fi

mkdir -p /usr/share/asteroids
cp -r images /usr/share/asteroids
cp -r sounds /usr/share/asteroids
cp -r music /usr/share/asteroids

cp asteroids-pygame-zero.py /usr/share/asteroids
chmod +x /usr/share/asteroids/asteroids-pygame-zero.py
cp asteroids-pygame-zero /usr/local/bin
chmod +x /usr/local/bin/asteroids-pygame-zero
cp asteroids-pygame-zero.desktop /usr/share/applications
