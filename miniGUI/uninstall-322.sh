#!/bin/bash

cd minigui-3.2.2

cd 3rd-parties

cd chipmunk-5.3.1
cat install_manifest.txt | sudo xargs rm
cat install_manifest.txt | xargs -L1 dirname | sudo xargs rmdir -p --ignore-fail-on-non-empty
cd ..
rm -rf chipmunk-5.3.1

for file in jpeg-7 zlib-1.2.2 mg-tools-3.2.1 libpng-1.2.37 freetype-2.6.1
do
    cd $file
    sudo make uninstall
    cd ..
    sudo rm -rf $file
done
cd ..

for comp in minigui-res-3.2.0 libminigui-3.2.2 libmgutils-1.2.0 libmgplus-1.4.0 libmgeff-1.2.1 libmgncs-1.2.2 libmgncs4touch-1.2.2 mg-samples-3.2.2
do
    cd $comp
    sudo make uninstall
    cd ..
    rm -rf $comp
done
