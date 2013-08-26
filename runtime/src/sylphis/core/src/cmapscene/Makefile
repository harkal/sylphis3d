

OBJS =  cmapscene.o \
        cmap.o \
        brushmodel.o

CPPFLAGS := $(CPPFLAGS) -I../ -I/usr/include/python2.1

all: $(OBJS)
	ar rcs cmapscene.a $(OBJS)

zlib.a:
	(cd zlib; make)

clean: oclean
	rm -f cmapscene.a

dep: cdep

include $(TOP_DIR)/Rules.make

