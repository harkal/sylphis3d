
OBJS =  brush.o brushplane.o bspnode.o entity.o shader.o bsptree.o \
	exception.o loader.o bspcreator.o mc.o boundbox.o \
        portal.o filterer.o portalcreator.o floodgate.o \
	plane.o polygon.o tjunctions.o tokenizer.o vertex.o mmgr.o bspwriter.o planemanager.o

GL_LIBS = -I/usr/X11R6/include -L/usr/lib -lGL -lglut -lGLU

GL_LIBS_WIN32 = -Ic:/MinGW/include -Lc:/MinGW/lib -lglu32 -lglut32 -lopengl32

ifeq ($(ARCH), _LINUX)
	LIBS = $(GL_LIBS)
else
	LIBS = $(GL_LIBS_WIN32)
endif

CPPFLAGS := $(CPPFLAGS) -I./ 


all: $(OBJS)
	#windres -o mcres.o mc.rc
	$(CPP) $(OPT) -o mc $(OBJS) $(LIBS) #mcres.o

clean: oclean
	rm -f mc 
	
dep: cdep

include ../Rules.make

