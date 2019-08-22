#!/bin/bash

help()
{
    echo ""
    echo "This script help you compile whole miniGUI library and its supporting libraries at one time!"
    echo "You don't specify the destination directory for installation, path /usr/local shall be chosen"
    echo "as default path".
    echo "You can also specify the destination directory where libraries is going to be installed according"
    echo "to following instruction:"
    echo ""
    echo "For instance: $0 ./linux    --  Install libraries and header files in folder linux of current directory."
    echo "              $0 output     --  Install libraries and header files in folder output of current directory."
    echo "              $0 /home/sean/usr -- Install libraries and header files in path /home/sean/usr."
    echo ""
}

if [ $# == 0 ]
then
    help

    install_path="/usr/local"
else
    install_path=$1
    if [ ${install_path:0:1} != "/" ]
    then
	install_path=`pwd`/$install_path
    fi
fi

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

tar xvzf minigui-3.2.2-suite.tgz

cd minigui-3.2.2

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

