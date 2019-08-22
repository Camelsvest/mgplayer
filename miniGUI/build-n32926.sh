#!/bin/bash

help()
{
    echo ""
    echo "This script help you compile whole miniGUI library and its supporting libraries at one time!"
    echo ""
    echo "Please specify the destination directory where libraries is going to be installed."
    echo "For instance: $0 ./nuvoton    --  Install libraries and header files in folder nuvoton of current directory."
    echo "              $0 output       --  Install libraries and header files in folder output of current directory."
    echo "              $0 /home/sean/usr -- Install libraries and header files in path /home/sean/usr."
    echo ""
}


if [ $# == 0 ]
then
    help
    exit 0
else
    install_path=$1
    if [ ${install_path:0:1} != "/" ]
    then
	install_path=`pwd`/$install_path
    fi
    
    echo ""
    echo "Now it's going to compile source code and install them into path $install_path"
    echo "Press any key to continue... "
    read Arg

fi


ARMTOOLS=arm-linux

export CC=$ARMTOOLS-gcc
export CXX=$ARMTOOLS-g++
export LD=$ARMTOOLS-ld
export NM=$ARMTOOLS-nm
export AS=$ARMTOOLS-as
export AR=$ARMTOOLS-ar
export RANLIB=$ARMTOOLS-ranlib
export STRIP=$ARMTOOLS-strip
export OBJDUMP=$ARMTOOLS-objdump

host=arm-linux
target=arm-linux

prefix=$install_path

export PKG_CONFIG_PATH=$prefix/lib/pkgconfig
export PATH=$prefix/bin:$PATH

if [ ! -f myconfig.sh ]; then
    cp config.sh myconfig.sh
fi

source myconfig.sh

runmode=$RUNMODE
nr_jobs=$NR_JOBS

mkdir minigui-3.2.2-n32926
tar xvzf minigui-3.2.2-suite.tgz -C minigui-3.2.2-n32926

cd minigui-3.2.2-n32926

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
	./configure --shared --prefix=$prefix
	make -j$nr_jobs
	make install

    elif [ "$file" == "chipmunk-5.3.1" ]; then
	 mkdir output; cd output
	 cmake -DCMAKE_INSTALL_PREFIX=$prefix ..;
	 make -j$nr_jobs	 
	 make install
	 cd ..
	 
#    elif [ "$file" == "freetype-2.6.1" ]; then
#	 mkdir output; cd output
#	 cmake -DCMAKE_INSTALL_PREFIX=$prefix ..
#	 make -j$nr_jobs
#
#	 # it's very strange that we need this patch in some Ubuntu, not all
#	 cd ..; patch -p1 < ../../../minigui-3.2.2-suite-patch.002; cd output
#	 make install
#	 cd ..
#	 
    else
         ./configure --prefix=$prefix --host=$host --target=$target
	 make -j$nr_jobs
	 make install
	 
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
#--enable-develmode  
./configure --prefix=$prefix --host=$host --target=$target --with-ttfsupport=ft2 --with-ttfcachesize=256 --with-ft2-includes=$prefix/include/freetype2 --with-runmode=$runmode --enable-pngsupport --disable-videoqvfb --disable-videopcxvfb --enable-tslibial --disable-cursor CFLAGS="-g -O0" LDFLAGS="-L$prefix/lib -L/ARM_Board/N329xx/N32926_V3.0_MW/W55FA92BSP2.6.35_160719/application/tslib-1.1/_tslib_install/lib" LIBS="-lpng -ljpeg -lfreetype -lm -lts -ldl"
make -j$nr_jobs; make install
cd ..

directories="minigui-res-3.2.0 libmgutils-1.2.0 libmgplus-1.4.0 libmgeff-1.2.1 libmgncs-1.2.2 libmgncs4touch-1.2.2"
for comp in $directories
do
    cd $comp
    if [ -f "./autogen.sh" ]
    then
        ./autogen.sh;
    fi

    if [ $comp == "minigui-res-3.2.0" ]
    then
	./configure --prefix=$prefix --host=$host --target=$target CFLAGS="-g -O0 -I$prefix/include/freetype2" CXXFLAGS="-g -O0 -I$prefix/include/freetype2"
    elif [ $comp == "libmgplus-1.4.0" ]
    then
	patch -p1 < ../../minigui-3.2.2-suite-patch.002
	./configure --prefix=$prefix --host=$host --target=$target --enable-ft2support CFLAGS="-g -O0 -I$prefix/include -I$prefix/include/freetype2" CXXFLAGS="-g -O0 -I$prefix/include -I$prefix/include/freetype2" LDFLAGS="-L$prefix/lib -L/ARM_Board/N329xx/N32926_V3.0_MW/W55FA92BSP2.6.35_160719/application/tslib-1.1/_tslib_install/lib"
    else
	./configure --prefix=$prefix --host=$host --target=$target CFLAGS="-g -O0 -I$prefix/include -I$prefix/include/freetype2" CXXFLAGS="-g -O0 -I$prefix/include -I$prefix/include/freetype2" LDFLAGS="-L$prefix/lib -L/ARM_Board/N329xx/N32926_V3.0_MW/W55FA92BSP2.6.35_160719/application/tslib-1.1/_tslib_install/lib"
    fi
    
    make -j$nr_jobs
    make install
    cd ..
done

# install mg-tools
cd 3rd-parties/mg-tools-3.2.1
./configure --prefix=$prefix --host=$host --target=$target CFLAGS="-g -O0 -I$prefix/include -I$prefix/include/freetype2" CXXFLAGS="-g -O0 -I$prefix/include -I$prefix/include/freetype2" LDFLAGS="-L$prefix/lib -L/ARM_Board/N329xx/N32926_V3.0_MW/W55FA92BSP2.6.35_160719/application/tslib-1.1/_tslib_install/lib"
make -j$nr_jobs; make install
cd ../..

# install mg-samples
cd mg-samples-3.2.2
if [ ! -f "./autogen.sh" ]
then
    ./autogen.sh;
fi
./configure --prefix=$prefix --host=$host --target=$target CFLAGS="-g -O0 -I$prefix/include -I$prefix/include/freetype2" CXXFLAGS="-g -O0 -I$prefix/include -I$prefix/include/freetype2" LDFLAGS="-L$prefix/lib -L/ARM_Board/N329xx/N32926_V3.0_MW/W55FA92BSP2.6.35_160719/application/tslib-1.1/_tslib_install/lib"
make -j$nr_jobs; make install
cd ..

cd ..



