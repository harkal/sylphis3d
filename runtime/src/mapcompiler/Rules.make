#
# General rules
#

.cpp.o:
	$(CPP) $(CPPFLAGS) $(OPT) -c -o $*.o $<
.c.s:
	$(CC) $(CFLAGS) $(OPT) -S -o $*.s $<
.c.o:
	$(CC) $(CFLAGS) $(OPT) -c -o $*.o $<

%.o: %.S
	$(CC) $(AFLAGS) -c -o $@ $<

oclean:
	rm -f $(OBJS)

cdep:
	gcc -M $(CFLAGS) $(CPPFLAGS) $(wildcard *.cpp) $(wildcard *.c) > .depend

depclean:
	@for i in `find -name .depend`; do \
		rm -f $$i; \
	done
	
#
# Standar exports
#
export TOP_DIR VSN OPT
export AS CPP CC CPPFLAGS CFLAGS AFLAGS LD ARCH

ifeq (.depend, $(wildcard .depend))
include .depend
endif

