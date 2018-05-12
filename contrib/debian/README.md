
Debian
====================
This directory contains files used to package zestd/zest-qt
for Debian-based Linux systems. If you compile zestd/zest-qt yourself, there are some useful files here.

## zest: URI support ##


zest-qt.desktop  (Gnome / Open Desktop)
To install:

	sudo desktop-file-install zest-qt.desktop
	sudo update-desktop-database

If you build yourself, you will either need to modify the paths in
the .desktop file or copy or symlink your zestqt binary to `/usr/bin`
and the `../../share/pixmaps/zest128.png` to `/usr/share/pixmaps`

zest-qt.protocol (KDE)

