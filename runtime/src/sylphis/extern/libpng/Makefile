
OBJS = \
pngerror.o  pngget.o  pngpread.o  pngrio.o    pngrutil.o  pngtest.o   pngvcrd.o  pngwrite.o  pngwutil.o \
png.o      pnggccrd.o  pngmem.o  pngread.o   pngrtran.o  pngset.o    pngtrans.o  pngwio.o   pngwtran.o

all: $(OBJS)
	$(AR) rcs libpng.a $(OBJS)

clean: oclean
	rm -f libpng.a

dep: cdep

include $(TOP_DIR)/Rules.make

