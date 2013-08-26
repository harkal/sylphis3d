#
# Makefile
#

PATCHLEVEL = 0
SUBLEVEL = 0
PRE = 0

VSN = $(VERSION).$(PATCHLEVEL).$(SUBLEVEL)

TOP_DIR = $(shell /bin/pwd)

ARCH = _LINUX

ifeq ($(shell uname|sed 's/MINGW32.*/WIN32/'), WIN32)
	ARCH = _WIN32
endif

#
# standar flags
#

#OPT = -O3 -g -pg -fprofile-arcs
#OPT = -O3 -finline-functions -mcpu=pentiumpro -march=pentiumpro -g3 
OPT = -g -D_NOINLINE -Wall
#OPT = -O3 -Wall -fbranch-probabilities -mcpu=i686 -march=i686 -ffast-math -fomit-frame-pointer -fforce-mem -fforce-addr -finline-functions -falign-functions=4 -mpreferred-stack-boundary=2 -fexpensive-optimizations -funroll-loops -fno-rtti 
OPT = -Wall -O3 -mcpu=pentiumpro -march=pentiumpro -fomit-frame-pointer -fforce-mem -fforce-addr -finline-functions -mpreferred-stack-boundary=2 -fexpensive-optimizations -funroll-loops -fno-rtti

AS = as
CC = gcc #gcc-3.0
CPP = g++ #g++-3.0
CFLAGS = -D$(ARCH) $(OPT) -I/c/MinGW/include -I/usr/X11R6/include
CPPFLAGS = -D$(ARCH) #-D_MMGR -D_MEMPROF
AFLAGS = -D__ASSEMBLER__ $(CFLAGS)
LD = ld

all: newbuild build.h
	(cd src; make)

loader:
	(cd src; make loader)

newbuild:
	expr 0`cat .build` + 1 > .build;

build.h: .build
	@echo \#define BUILD_NUMBER \"`cat .build`\" > src/build.h
	@echo \#define BUILD_DATE \"`date "+%d %b %Y - %T"`\" >> src/build.h

distclean: clean depclean

indent:
	indent -br -ce -npcs -nprs -npsl -nut -ts4 -i4 src/*.h src/*.cpp

clean: 
	(cd src; make clean)

dep: 
	(cd src; make dep)

include $(TOP_DIR)/Rules.make

