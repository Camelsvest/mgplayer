# !bin/bash

install_path=/usr/local

sudo rm -rf $install_path/include/minigui
sudo rm -rf $install_path/include/mgutils
sudo rm -rf $install_path/include/mgplus
sudo rm -rf $install_path/include/mgncs
sudo rm -rf $install_path/include/mgeff
sudo rm -rf $install_path/include/libpng12
sudo rm -rf $install_path/include/freetype2
sudo rm $install_path/include/zconf.h
sudo rm  $install_path/include/zlib.h

sudo rm  $install_path/lib/libfreetype.*
sudo rm  $install_path/lib/libpng.*

sudo rm -rf $install_path/share/minigui
