
OBJS = \
adler32.o crc32.o gzio.o infcodes.o inflate.o infutil.o trees.o zutil.o \
compress.o deflate.o infblock.o inffast.o inftrees.o maketree.o uncompr.o unzip.o

all: $(OBJS)
	$(AR) rcs zlib.a $(OBJS)

clean: oclean
	rm -f zlib.a

dep: cdep

include $(TOP_DIR)/Rules.make

