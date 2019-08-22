# Makefile for project  mgplayer
#
MKFILE_PATH := $(abspath $(lastword $(MAKEFILE_LIST))) 
CUR_MKFILE_DIRECTORY := $(patsubst %/,%,$(dir $(MKFILE_PATH)))

ifeq ($(MAKECMDGOALS), nuvoton)
	CROSS_COMPILER := arm-linux-
	TARGET := mgplayer-n32926
	MINIGUI_DIR := $(CUR_MKFILE_DIRECTORY)/build/n32926
#	CINCLUDE_PATH := "/usr/local/arm_linux_4.8/arm-nuvoton-linux-uclibceabi/include/linux"
#	CPPINCLUDE_PATH := "/usr/local/arm_linux_4.8/arm-nuvoton-linux-uclibceabi/include/c++/4.8.4"
else ifeq ($(MAKECMDGOALS), linux)
	CROSS_COMPILER :=
	TARGET := mgplayer-linux
	MINIGUI_DIR := /usr/local
else ifeq ($(MAKECMDGOALS), clean)
	TARGET := $(wildcard ./mgplayer-*)
else ifeq ($(MAKECMDGOALS), )
	TARGET :=
endif

C++ := $(CROSS_COMPILER)g++
AR  := $(CROSS_COMPILER)ar
LD  := $(CROSS_COMPILER)ld

MINIGUI_INCLUDE=-I$(MINIGUI_DIR)/include
#
MINIGUI_LIB=-L$(MINIGUI_DIR)/lib -lminigui_ths -lmgncs -lmgplus -lmgutils -lpng -ljpeg -lfreetype

LOGGING_INCLUDE=-Ilogging
LOGGING_LIB= -L./logging -llogging

DEFINES := -D_MGNCS_RDR_SKIN
INCLUDE := -Iinclude $(LOGGING_INCLUDE) $(MINIGUI_INCLUDE)

#-nostdinc++ -x c++ -E -v
CPPFLAGS := -g -O0 -Wall -pthread $(DEFINES) $(INCLUDE)

LDFLAGS := -pthread

ifeq ($(MAKECMDGOALS), nuvoton)
	LIBS := $(LOGGING_LIB) $(MINIGUI_LIB) -L/ARM_Board/N329xx/N32926_V3.0_MW/W55FA92BSP2.6.35_160719/application/tslib-1.1/_tslib_install/lib -lts -lm -ldl
else
	LIBS := $(LOGGING_LIB) $(MINIGUI_LIB) -lm -ldl
endif

SOURCE  := $(wildcard src/*.cpp)
OBJS    := $(patsubst %.cpp,%.o,$(SOURCE))

LOGGING = liblogging.a

.PHONY : nuvoton linux objs clean cscope help upload

help :
	@echo ""
	@echo "Usage: You must specify a target for MAKE action. There are following targets:"
	@echo "		make nuvoton	-- Compiling for NUVOTON N32923 platform"
	@echo "		make linux	-- Compiling for Linux platform"
	@echo "		make objs	-- Only compiling, but not generate final executive program"
	@echo "		make clean	-- Clean all generated object files"
	@echo "		make rebuild	-- Clean all and regenerate all object files and program"
	@echo "		make cscope	-- Build index file of cscope"
	@echo ""

nuvoton : $(TARGET)
	@echo ""
	@echo "Succeed to compile target for NUVOTON N32926 platform!"
	@echo ""

linux : $(TARGET)
	@echo ""
	@echo "Succeed to compile target for Linux platform!"
	@echo ""

objs : $(OBJS)

%.o : %.cpp
	$(C++) $(CPPFLAGS) -c $< -o $@

clean:
	@rm -rf src/*.o
	@rm -rf $(TARGET)
	make -C ./logging clean

$(TARGET) : $(OBJS) $(LOGGING)
	$(C++) $(LDFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

$(LOGGING):
ifeq ($(MAKECMDGOALS),nuvoton)
	make -C ./logging CROSS_COMPILER=arm-linux-
else
	make -C ./logging
endif

cscope:
	find ./ -type f -name "*.[hc]" -o -name "*.cpp" > cscope.files
	cscope -bqRk -F ./cscope.files

upload: nuvoton
	cp ./mgplayer-n32926 ~/nfsshare
	@if test ! -d /home/urmet/nfsshare/minigui/lib; then\
		mkdir -p ~/nfsshare/minigui/lib;\
	fi

	cp ./build/n32926/lib/libz.so.1.2.2 ~/nfsshare/minigui/lib
	@if test ! -f ~/nfsshare/minigui/lib/libz.so.1; then\
		ln -s ~/nfsshare/minigui/lib/libz.so.1.2.2 ~/nfsshare/minigui/lib/libz.so.1;\
	fi

	@if test ! -f ~/nfsshare/minigui/lib/libz.so; then\
		ln -s ~/nfsshare/minigui/lib/libz.so.1.2.2 ~/nfsshare/minigui/lib/libz.so;\
	fi

	cp ./build/n32926/lib/libpng12.so.0.37.0 ~/nfsshare/minigui/lib
	@if test ! -f ~/nfsshare/minigui/lib/libpng12.so.0; then\
		ln -s ~/nfsshare/minigui/lib/libpng12.so.0.37.0 ~/nfsshare/minigui/lib/libpng12.so.0;\
	fi
	@if test ! -f ~/nfsshare/minigui/lib/libpng12.so; then\
		ln -s ~/nfsshare/minigui/lib/libpng12.so.0.37.0 ~/nfsshare/minigui/lib/libpng12.so;\
	fi

	cp ./build/n32926/lib/libjpeg.so.7.0.0  ~/nfsshare/minigui/lib
	@if test ! -f ~/nfsshare/minigui/lib/libjpeg.so.7; then\
		ln -s ~/nfsshare/minigui/lib/libjpeg.so.7.0.0 ~/nfsshare/minigui/lib/libjpeg.so.7;\
	fi
	@if test ! -f ~/nfsshare/minigui/lib/libjpeg.so; then\
		ln -s ~/nfsshare/minigui/lib/libjpeg.so.7.0.0 ~/nfsshare/minigui/lib/libjpeg.so;\
	fi

	cp ./build/n32926/lib/libminigui_ths-3.2.so.0.2.0 ~/nfsshare/minigui/lib
	@if test ! -f ~/nfsshare/minigui/lib/libminigui_ths-3.2.so.0; then\
		ln -s ~/nfsshare/minigui/lib/libminigui_ths-3.2.so.0.2.0 ~/nfsshare/minigui/lib/libminigui_ths-3.2.so.0;\
	fi

	cp ./build/n32926/lib/libmgncs4touch-1.2.so.2.0.0 ~/nfsshare/minigui/lib
	@if test ! -f ~/nfsshare/minigui/lib/libmgncs4touch.so; then\
		ln -s ~/nfsshare/minigui/lib/libmgncs4touch-1.2.so.2.0.0 ~/nfsshare/minigui/lib/libmgncs4touch.so;\
	fi

	cp ./build/n32926/lib/libmgncs-1.2.so.1.1.0 ~/nfsshare/minigui/lib
	@if test ! -f ~/nfsshare/minigui/lib/libmgncs-1.2.so.1; then\
		ln -s ~/nfsshare/minigui/lib/libmgncs-1.2.so.1.1.0 ~/nfsshare/minigui/lib/libmgncs-1.2.so.1;\
	fi

	cp ./build/n32926/lib/libmgplus-1.4.so.0.0.0 ~/nfsshare/minigui/lib
	@if test ! -f ~/nfsshare/minigui/lib/libmgplus-1.4.so.0; then\
		ln -s ~/nfsshare/minigui/lib/libmgplus-1.4.so.0.0.0 ~/nfsshare/minigui/lib/libmgplus-1.4.so.0;\
	fi

	cp ./build/n32926/lib/libmgutils-1.2.so.0.0.0 ~/nfsshare/minigui/lib
	@if test ! -f ~/nfsshare/minigui/lib/libmgutils-1.2.so.0; then\
		ln -s ~/nfsshare/minigui/lib/libmgutils-1.2.so.0.0.0 ~/nfsshare/minigui/lib/libmgutils-1.2.so.0;\
	fi

	cp ./build/n32926/lib/libchipmunk.so.5.1 ~/nfsshare/minigui/lib
	@if test ! -f ~/nfsshare/minigui/lib/libchipmunk.so; then\
		ln -s ~/nfsshare/minigui/lib/libchipmunk.so.5.1 ~/nfsshare/minigui/lib/libchipmunk.so;\
	fi

	cp ./build/n32926/lib/libfreetype.so.6.12.1 ~/nfsshare/minigui/lib
	@if test ! -f ~/nfsshare/minigui/lib/libfreetype.so; then\
		ln -s ~/nfsshare/minigui/lib/libfreetype.so.6.12.1 ~/nfsshare/minigui/lib/libfreetype.so;\
	fi
	@if test ! -f ~/nfsshare/minigui/lib/libfreetype.so.6; then\
		ln -s ~/nfsshare/minigui/lib/libfreetype.so.6.12.1 ~/nfsshare/minigui/lib/libfreetype.so.6;\
	fi
