#!/bin/bash

install_path="/usr/local"
echo ""
echo "Now it's going to compile source code and install them into path $install_path"
echo "Press any key to continue... "
read Arg


if [ ! -f myconfig.sh ]; then
    cp config.sh myconfig.sh
fi

source myconfig.sh

runmode=$RUNMODE
nr_jobs=$NR_JOBS

mkdir minigui-3.2.2-linux
tar xvzf minigui-3.2.2-suite.tgz -C ./minigui-3.2.2-linux

cd minigui-3.2.2-linux

cd 3rd-parties

#"zlib-1.2.2.tar.gz jpegsrc.v7.tar.gz libpng-1.2.37.tar.gz chipmunk-5.3.1.tar.gz freetype-2.6.1.tar.gz mg-tools-3.2.1.tar.gz"
files=`find ./ -maxdepth 1 -name "*.tar.gz"`

for file in $files; do
    tar xvzf $file
done

directories="zlib-1.2.2 jpeg-7 libpng-1.2.37 freetype-2.6.1 chipmunk-5.3.1"

for file in $directories; do
    cd $file
    echo `pwd`

    if [ "$file" == "zlib-1.2.2" ]; then
	#./configure --shared
	#make -j$nr_jobs; sudo make install

	# we use system os ZLIB to avoid some conflication in i386/x86_64 OS
	sudo apt-get install zlib1g-dev -y

    elif [ "$file" == "chipmunk-5.3.1" ]; then
	 cd $file

	 mkdir build; cd build
	 cmake ..;
	 make -j$nr_jobs;	 
	 sudo make install
	 cd ..

#    elif [ "$file" == "freetype-2.6.1" ]; then
#	 cd $file
#
#	 mkdir build; cd build
#	 cmake ..;
#	 make -j$nr_jobs;
#
#	 # it's very strange that we need this patch in some Ubuntu, not all
#	 cd ..; patch -p1 < ../../../minigui-3.2.2-suite-patch.002; cd  build
#	 sudo make install
#	 cd ..
	 
    else
         ./configure --prefix=$install_path
	 make -j$nr_jobs; sudo make install	 
    fi   
    
    cd ..
done

cd .. # quit from folder 3rd-parties

#"minigui-res-3.2.0.tar.gz libminigui-3.2.2.tar.gz libmgutils-1.2.0.tar.gz libmgplus-1.4.0.tar.gz libmgeff-1.2.1.tar.gz libmgncs-1.2.2.tar.gz libmgncs4touch-1.2.2.tar.gz mg-samples-3.2.2.tar.gz"
files=`find ./ -maxdepth 1 -name "*.tar.gz"`
for file in $files; do
    tar xvzf $file
done

echo "Apply patch !"
patch -p1 < ../minigui-3.2.2-suite-patch.001
patch -p1 < ../minigui-3.2.2-suite-patch.003

cd libminigui-3.2.2
./configure --prefix=$install_path --with-ttfsupport=ft2 --with-ttfcachesize=256 --with-ft2-includes=/usr/local/include/freetype2 --enable-develmode --with-runmode=$runmode CFLAGS="-g -O0"
make -j$nr_jobs; sudo make install
cd ..

directories="minigui-res-3.2.0 libmgutils-1.2.0 libmgplus-1.4.0 libmgeff-1.2.1 libmgncs-1.2.2 libmgncs4touch-1.2.2"
for comp in $directories; do
    cd $comp
    if [ ! -f "./autogen.sh" ]; then
        ./autogen.sh;
    fi
    ./configure --prefix=$install_path CFLAGS="-g -O0 -I/usr/local/include/freetype2" CXXFLAGS="-g -O0 -I/usr/local/include/freetype2"
    make -j$nr_jobs; sudo make install
    cd ..
done

cd 3rd-parties/mg-tools-3.2.1
./configure --prefix=$install_path
make -j$nr_jobs; sudo make install
cd ../..

cd mg-samples-3.2.2
if [ ! -f "./autogen.sh" ]; then
    ./autogen.sh;
fi
./configure --prefix=$install_path --enable-develmode CFLAGS="-g -O0" CXXFLAGS="-g -O0"
 make -j$nr_jobs; sudo make install
cd ..

