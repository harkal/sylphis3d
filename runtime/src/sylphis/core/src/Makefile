
OBJS = \
actor.o \
actormover.o \
animationblender.o \
bonekeyframe.o \
bonetrack.o \
boundbox.o \
beamtree.o \
camera.o \
client.o \
commands.o \
configfile.o \
console.o \
controler.o \
cvars.o \
enginecomponent.o \
engine.o \
eventlistener.o \
exception.o \
font.o \
frustum.o \
ftlibrary.o \
gutils.o \
imagefile.o \
inputdevice.o \
interpolator.o \
joint.o \
light.o \
main.o \
matrix.o \
md3model.o \
mesh.o \
meshloader3ds.o \
meshloadermd2.o \
model.o \
modelmanager.o \
myglu.o \
normalmap.o \
openglext.o \
plane.o \
pointer.o \
polygon.o \
quaternion.o \
refobject.o \
renderable.o \
renderablemd2.o \
renderablemultimesh.o \
renderablequeuemember.o \
rendererARB.o \
renderer.o \
rendererGL.o \
rendererNULL.o \
renderingview.o \
renderingviewx11.o \
renderqueue.o \
resource.o \
resourcemanager.o \
rigidbody.o \
rigidbox.o \
rigidcapsule.o \
rigidcyl.o \
rigidsphere.o \
scenemanager.o \
scenemd3.o \
scenemodel.o \
scenenode.o \
scenenodemember.o \
sceneobject.o \
serverclientbase.o \
server.o \
shader.o \
shadowvolume.o \
silhouette.o \
singleton.o \
skelanimation.o \
skeleton.o \
skeletoninstance.o \
skelmodel.o \
texture.o \
timer.o \
tokenizer.o \
vector.o \
vfs.o

GL_LIBS = -L/usr/lib -lGL
FT_LIBS = `freetype-config --libs`
ODE_LIBS = -L/home/harkal/ode-0.039-test/lib -lode -lopcode
X_LIBS = -L/usr/local/lib -ldl -L/usr/X11R6/lib -lX11 -lXext
LIBS = -L/usr/local/lib/python2.2/config/ -lpython2.2 -lutil -Xlinker -export-dynamic $(FT_LIBS) $(X_LIBS) $(GL_LIBS) $(ODE_LIBS)
FTFLAGS = `freetype-config --cflags`
CPPFLAGS := $(CPPFLAGS) -I./ -I../include -I../../include -I/usr/X11R6/include  $(FTFLAGS)
	
all: $(OBJS) zlib.a libpng.a cmapscene.a python.a
	$(CPP) $(OPT) -o sylphis $(OBJS) cmapscene/cmapscene.a python/python.a zlib/zlib.a libpng/libpng.a /usr/lib/libjpeg.a $(LIBS)

cmapscene.a:
	(cd cmapscene; make)

python.a:
	(cd python; make)
    
zlib.a:
	(cd zlib; make)

libpng.a:
	(cd libpng; make)

clean: oclean
	rm -f sylphis
	(cd zlib; make clean)
	(cd libpng; make clean)
	(cd cmapscene; make clean)
	(cd python; make clean)

dep: cdep
	(cd zlib; make dep)
	(cd libpng; make dep)
	(cd cmapscene; make dep)
	(cd python; make dep)

include ../Rules.make

