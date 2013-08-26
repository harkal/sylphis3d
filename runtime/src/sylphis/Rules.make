
#
# General rules
#

.cpp.o:
	$(CPP) $(CPPFLAGS) -c -o $*.o $<
.c.s:
	$(CC) $(CFLAGS) $(OPT) -S -o $*.s $<
.c.o:
	$(CC) $(CFLAGS) $(OPT) -c -o $*.o $<

%.o: %.S
	$(CC) $(AFLAGS) -c -o $@ $<

oclean:
	rm -f $(OBJS)
	rm -f *.da

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
export AS CPP CC PRECOMP CPPFLAGS CFLAGS AFLAGS LD

ifeq (.depend, $(wildcard .depend))
include .depend
endif

