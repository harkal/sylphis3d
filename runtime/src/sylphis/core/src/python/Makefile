

OBJS =  sylphis_wrap.o

CPPFLAGS := $(CPPFLAGS) -I../

all: $(OBJS)
	ar rcs python.a $(OBJS)

sylphis_wrap.cpp: 
	(cd ../../ ; swig -c++ -python -o src/python/sylphis_wrap.cpp $(PRECOMP) src/python/sylphis.i)

clean: oclean
	rm -f python.a
	rm -f sylphis_wrap.cpp

dep: 

include $(TOP_DIR)/Rules.make

