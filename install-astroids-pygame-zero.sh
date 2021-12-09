#!/usr/bin/env bash
if [ 0 != $UID ]; then
    echo "This script must be run as root with sudo.";
    exit 1;
fi

mkdir -p /usr/share/astroids
cp -r images /usr/share/astroids
cp -r sounds /usr/share/astroids
cp -r music /usr/share/astroids

cp astroids-pygame-zero.py /usr/share/astroids
chmod +x /usr/share/astroids/astroids-pygame-zero.py
cp astroids-pygame-zero /usr/local/bin
chmod +x /usr/local/bin/astroids-pygame-zero
cp astroids-pygame-zero.desktop /usr/share/applications
