PROJECT(Sylphis3D)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)

#INCLUDE(FindSWIG)

#Name for the hllib is different depending on the platform compiled on
SET(HK_LIB_NAME "")

#Same goes for the freetype2 lib
SET(FREETYPE2_LIB_NAME "")

#Source files fir sylphis3d core
SET(SYLPHIS_CORE_SOURCE)

#Source files for the zlib hklib external dependency
SET(ZLIB_SOURCE)

#Source files for the python
SET(PYTHON_SOURCE)

#Source files for libpng
SET(LIBPNG_SOURCE)

#Source files for cmapscene
SET(CMAPSCENE_SOURCE)

#Source files for sound
SET(SOUND_SOURCE)

#All the source files neede for compiling the sylphis executable
SET(ALL_SOURCES)

#SWIG interface input file
#SET(SWIG_INTERFACE_FILE)

#SWIG C++ output file
#SET(SWIG_OUTPUT_FILE)

INCLUDE_DIRECTORIES(
./core/include
./ode/include
./core/src
./extern
./extern/include
./extern/include/freetype2
./hklib/include
./hklib/extern/zlib
./core/src/python
./extern/libpng
.)

#FIND_PACKAGE(SWIG REQUIRED)
#INCLUDE(${SWIG_USE_FILE})

FIND_PACKAGE(PythonLibs REQUIRED)
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})

IF(UNIX)
  IF(APPLE)
    MESSAGE(FATAL_ERROR "Mac OS is not a supported platform!")
  ELSE(APPLE)
    MESSAGE(STATUS "Building for Linux or Linux compatible.")
    ADD_DEFINITIONS(-D_LINUX)
    SET(HK_LIB_NAME "hk")
    SET(FREETYPE2_LIB_NAME "freetype")
    SET(SYLPHIS_CORE_SOURCE
    ${SYLPHIS_CORE_SOURCE}
    ./core/src/renderingviewx11.cpp)
  ENDIF(APPLE)
ELSE(UNIX)
  IF(WIN32)
    MESSAGE(STATUS "Builing for Windows")
    ADD_DEFINITIONS(-D_WINDOWS -DWIN32)
    SET(HK_LIB_NAME "hklib")
    SET(FREETYPE2_LIB_NAME "freetype2")
    SET(SYLPHIS_CORE_SOURCE
    ${SYLPHIS_CORE_SOURCE}
    ./core/src/renderingviewwin32.cpp)
  ELSE(WIN32)
    MESSAGE(FATAL_ERROR "Unknown/Unsupported platform!")
  ENDIF(WIN32)
ENDIF(UNIX)

IF(CMAKE_BUILD_TYPE STREQUAL "")
    MESSAGE(STATUS "Warning: No build type specified, defaulting to release.")
    SET(CMAKE_BUILD_TYPE "Debug")
ENDIF(CMAKE_BUILD_TYPE STREQUAL "")

IF(CMAKE_BUILD_TYPE STREQUAL "Release")
    MESSAGE(STATUS "Building release build.")
    ADD_DEFINITIONS(-DNDEBUG)
    SET(EXECUTABLE_OUTPUT_PATH ./Release)
    SET(LIBRARY_OUTPUT_PATH ./Release)
ENDIF(CMAKE_BUILD_TYPE STREQUAL "Release")

IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
    MESSAGE(STATUS "Building debug build.")
    ADD_DEFINITIONS(-D_DEBUG -DDEBUG)
    SET(EXECUTABLE_OUTPUT_PATH ./Debug)
    SET(LIBRARY_OUTPUT_PATH ./Debug)
ENDIF(CMAKE_BUILD_TYPE STREQUAL "Debug")
    
ADD_DEFINITIONS(-DGLEW_STATIC -D_CRT_SECURE_NO_DEPRECATE)

#All the libraries sylphis needs for linking
SET(SYLPHIS_LIBRARIES
GL
GLU
GLEW
#${HK_LIB_NAME}
${FREETYPE2_LIB_NAME}
ogg
vorbis
vorbisenc
vorbisfile
util
openal
alut
ode
jpeg
${PYTHON_LIBRARY}
)

SET(SYLPHIS_CORE_SOURCE
${SYLPHIS_CORE_SOURCE}
./core/src/actor.cpp 
./core/src/actormover.cpp
./core/src/animationblender.cpp
./core/src/beamtree.cpp
./core/src/bonekeyframe.cpp
./core/src/bonetrack.cpp
./core/src/boundbox.cpp
./core/src/buffer.cpp
./core/src/cache.cpp
./core/src/camera.cpp
./core/src/client.cpp
./core/src/commands.cpp
./core/src/configfile.cpp
./core/src/controler.cpp
./core/src/debugdrawing.cpp
./core/src/engine.cpp
./core/src/eventlistener.cpp
./core/src/font.cpp
./core/src/frustum.cpp
./core/src/ftlibrary.cpp
./core/src/glrenderer/glrendersystem.cpp
./core/src/glrenderer/glrendertexture.cpp
./core/src/glrenderer/gltexture.cpp
./core/src/gutils.cpp
./core/src/imagefile.cpp
./core/src/indexbuffer.cpp
./core/src/inputdevice.cpp
./core/src/interpolator.cpp
./core/src/joint.cpp # <- Ask harkal about strange conversion
./core/src/light.cpp
./core/src/main.cpp
./core/src/material.cpp
./core/src/materialparser.cpp 
./core/src/matrix.cpp
./core/src/matrix3.cpp
./core/src/md3model.cpp
./core/src/mesh.cpp
./core/src/meshloader3ds.cpp
./core/src/model.cpp
./core/src/modelmanager.cpp
./core/src/myglu.c
./core/src/openglext.cpp
./core/src/particlesystem.cpp
./core/src/physicsenvironment.cpp
./core/src/plane.cpp
./core/src/polygon.cpp
./core/src/quaternion.cpp
./core/src/renderable.cpp
./core/src/renderablemultimesh.cpp
./core/src/renderablequeuemember.cpp
./core/src/renderchunk.cpp
./core/src/renderer.cpp
./core/src/rendererARB2.cpp
./core/src/renderingview.cpp
./core/src/renderqueue.cpp
./core/src/rendersystem.cpp
./core/src/rendertexture.cpp
./core/src/resource.cpp
./core/src/resourcemanager.cpp
./core/src/rigidbody.cpp
./core/src/rigidbodystatic.cpp
./core/src/rigidbox.cpp #Error starts here
./core/src/rigidcapsule.cpp
./core/src/rigidcomplex.cpp
./core/src/rigidcyl.cpp
./core/src/rigidsphere.cpp #Error ends here
./core/src/scenemanager.cpp
./core/src/scenemd3.cpp
./core/src/scenemodel.cpp
./core/src/scenenode.cpp
./core/src/scenenodemember.cpp
./core/src/sceneobject.cpp
./core/src/shadowvolume.cpp
./core/src/silhouette.cpp
./core/src/skelanimation.cpp
./core/src/skeleton.cpp
./core/src/skeletoninstance.cpp
./core/src/skelmodel.cpp
./core/src/smfmodel.cpp
./core/src/splineinterpolator.cpp
./core/src/sylphis.rc
./core/src/texture.cpp
./core/src/texturefont.cpp
./core/src/timer.cpp
./core/src/tokenizer.cpp
./core/src/vector.cpp
./core/src/vertexbuffer.cpp)

SET(ZLIB_SOURCE
${ZLIB_SOURCE}
./hklib/extern/zlib/adler32.c
./hklib/extern/zlib/compress.c
./hklib/extern/zlib/crc32.c
./hklib/extern/zlib/deflate.c
./hklib/extern/zlib/gzio.c
./hklib/extern/zlib/infblock.c
./hklib/extern/zlib/infcodes.c
./hklib/extern/zlib/inffast.c
./hklib/extern/zlib/inflate.c
./hklib/extern/zlib/inftrees.c
./hklib/extern/zlib/infutil.c
./hklib/extern/zlib/maketree.c
./hklib/extern/zlib/trees.c
./hklib/extern/zlib/uncompr.c
./hklib/extern/zlib/unzip.cpp
./hklib/extern/zlib/zutil.c)

SET(PYTHON_SOURCE
${PYTHON_SOURCE}
./core/src/python/mthread.cpp
./core/src/python/pyconsole.cpp
./core/src/python/pyhelper.cpp
./core/src/python/sylphis_wrap.cpp
./core/src/python/waitable.cpp)

SET(LIBPNG_SOURCE
${LIBPNG_SOURCE}
./extern/libpng/example.c
./extern/libpng/png.c
./extern/libpng/pngerror.c
./extern/libpng/pnggccrd.c
./extern/libpng/pngget.c
./extern/libpng/pngmem.c
./extern/libpng/pngpread.c
./extern/libpng/pngread.c
./extern/libpng/pngrio.c
./extern/libpng/pngrtran.c
./extern/libpng/pngrutil.c
./extern/libpng/pngset.c
./extern/libpng/pngtest.c
./extern/libpng/pngtrans.c
./extern/libpng/pngvcrd.c
./extern/libpng/pngwio.c
./extern/libpng/pngwrite.c
./extern/libpng/pngwtran.c
./extern/libpng/pngwutil.c)

SET(CMAPSCENE_SOURCE
${CMAPSCENE_SOURCE}
./core/src/cmapscene/brushmodel.cpp
./core/src/cmapscene/cmap.cpp
./core/src/cmapscene/cmapscene.cpp # <- Talk to harkal about CVector3 references
)

SET(SOUND_SOURCE
${SOUND_SOURCE}
./core/src/sound/soundbuffer.cpp
./core/src/sound/soundenv.cpp
./core/src/sound/soundlistener.cpp
./core/src/sound/soundobject.cpp
./core/src/sound/soundogg.cpp
./core/src/sound/soundsource.cpp
./core/src/sound/soundwave.cpp)

SET(ALL_SOURCES
${ALL_SOURCES}
${SYLPHIS_CORE_SOURCE}
${ZLIB_SOURCE}
${PYTHON_SOURCE}
${LIBPNG_SOURCE}
${CMAPSCENE_SOURCE}
${SOUND_SOURCE})

SET(HKLIB_SOURCE
hklib/src/classinfo.cpp
hklib/src/console.cpp
hklib/src/exception.cpp
hklib/src/pointer.cpp
hklib/src/refobject.cpp
hklib/src/serialize.cpp
hklib/src/serializer.cpp
hklib/src/singleton.cpp
hklib/src/vfs.cpp
)
#SET(SWIG_INTERFACE_FILE ./core/src/python/sylphis.i)
#SET(SWIG_OUTPUT_FILE ./core/src/python/sylphis_wrap.cpp) 

#SET_SOURCE_FILES_PROPERTIES(${SWIG_INTERFACE_FILE} PROPERTIES CPLUSPLUS ON)
#SET(SWIG_FLAGS "-o ${SWIG_OUTPUT_FILE}")
#SWIG_ADD_MODULE(sylphis_swig Python ${SWIG_INTERFACE_FILE})
#SWIG_LINK_LIBRARIES(sylphis_swig ${PYTHON_LIBRARIES})

ADD_EXECUTABLE(sylphis ${ALL_SOURCES})
ADD_LIBRARY(hk ${HKLIB_SOURCE})
TARGET_LINK_LIBRARIES(sylphis hk ${SYLPHIS_LIBRARIES})

#Make sure the swig generated file is generated before compiling sylphis
#ADD_DEPENDENCIES(sylphis sylphis_swig)

PROJECT(Sylphis3D)

SET(CMAKE_INCLUDE_CURRENT_DIR ON)

#INCLUDE(FindSWIG)

#Name for the hllib is different depending on the platform compiled on
SET(HK_LIB_NAME "")

#Same goes for the freetype2 lib
SET(FREETYPE2_LIB_NAME "")

#Source files fir sylphis3d core
SET(SYLPHIS_CORE_SOURCE)

#Source files for the zlib hklib external dependency
SET(ZLIB_SOURCE)

#Source files for the python
SET(PYTHON_SOURCE)

#Source files for libpng
SET(LIBPNG_SOURCE)

#Source files for cmapscene
SET(CMAPSCENE_SOURCE)

#Source files for sound
SET(SOUND_SOURCE)

#All the source files neede for compiling the sylphis executable
SET(ALL_SOURCES)

#SWIG interface input file
#SET(SWIG_INTERFACE_FILE)

#SWIG C++ output file
#SET(SWIG_OUTPUT_FILE)

INCLUDE_DIRECTORIES(
./core/include
./core/src
./extern
./extern/include
./extern/include/freetype2
./hklib/include
./hklib/extern/zlib
./core/src/python
./extern/libpng
.)

#FIND_PACKAGE(SWIG REQUIRED)
#INCLUDE(${SWIG_USE_FILE})

FIND_PACKAGE(PythonLibs REQUIRED)
INCLUDE_DIRECTORIES(${PYTHON_INCLUDE_PATH})

IF(UNIX)
  IF(APPLE)
    MESSAGE(FATAL_ERROR "Mac OS is not a supported platform!")
  ELSE(APPLE)
    MESSAGE(STATUS "Building for Linux or Linux compatible.")
    ADD_DEFINITIONS(-D_LINUX)
    SET(HK_LIB_NAME "hk")
    SET(FREETYPE2_LIB_NAME "freetype")
    SET(SYLPHIS_CORE_SOURCE
    ${SYLPHIS_CORE_SOURCE}
    ./core/src/renderingviewx11.cpp)
  ENDIF(APPLE)
ELSE(UNIX)
  IF(WIN32)
    MESSAGE(STATUS "Builing for Windows")
    ADD_DEFINITIONS(-D_WINDOWS -DWIN32)
    SET(HK_LIB_NAME "hklib")
    SET(FREETYPE2_LIB_NAME "freetype2")
    SET(SYLPHIS_CORE_SOURCE
    ${SYLPHIS_CORE_SOURCE}
    ./core/src/renderingviewwin32.cpp)
  ELSE(WIN32)
    MESSAGE(FATAL_ERROR "Unknown/Unsupported platform!")
  ENDIF(WIN32)
ENDIF(UNIX)

IF(CMAKE_BUILD_TYPE STREQUAL "")
    MESSAGE(STATUS "Warning: No build type specified, defaulting to release.")
    SET(CMAKE_BUILD_TYPE "Debug")
ENDIF(CMAKE_BUILD_TYPE STREQUAL "")

IF(CMAKE_BUILD_TYPE STREQUAL "Release")
    MESSAGE(STATUS "Building release build.")
    ADD_DEFINITIONS(-DNDEBUG)
    SET(EXECUTABLE_OUTPUT_PATH ./Release)
    SET(LIBRARY_OUTPUT_PATH ./Release)
ENDIF(CMAKE_BUILD_TYPE STREQUAL "Release")

IF(CMAKE_BUILD_TYPE STREQUAL "Debug")
    MESSAGE(STATUS "Building debug build.")
    ADD_DEFINITIONS(-D_DEBUG)
    SET(EXECUTABLE_OUTPUT_PATH ./Debug)
    SET(LIBRARY_OUTPUT_PATH ./Debug)
ENDIF(CMAKE_BUILD_TYPE STREQUAL "Debug")
    
ADD_DEFINITIONS(-DGLEW_STATIC -D_CRT_SECURE_NO_DEPRECATE)

#All the libraries sylphis needs for linking
SET(SYLPHIS_LIBRARIES
GL
GLU
GLEW
${HK_LIB_NAME}
${FREETYPE2_LIB_NAME}
ogg
openal
alut
ode
jpeg
${PYTHON_LIBRARY}
)

SET(SYLPHIS_CORE_SOURCE
${SYLPHIS_CORE_SOURCE}
./core/src/actor.cpp 
./core/src/actormover.cpp
./core/src/animationblender.cpp
./core/src/beamtree.cpp
./core/src/bonekeyframe.cpp
./core/src/bonetrack.cpp
./core/src/boundbox.cpp
./core/src/buffer.cpp
./core/src/cache.cpp
./core/src/camera.cpp
./core/src/client.cpp
./core/src/commands.cpp
./core/src/configfile.cpp
./core/src/controler.cpp
./core/src/debugdrawing.cpp
./core/src/engine.cpp
./core/src/eventlistener.cpp
./core/src/font.cpp
./core/src/frustum.cpp
./core/src/ftlibrary.cpp
./core/src/glrenderer/glrendersystem.cpp
./core/src/glrenderer/glrendertexture.cpp
./core/src/glrenderer/gltexture.cpp
./core/src/gutils.cpp
./core/src/imagefile.cpp
./core/src/indexbuffer.cpp
./core/src/inputdevice.cpp
./core/src/interpolator.cpp
./core/src/joint.cpp # <- Ask harkal about strange conversion
./core/src/light.cpp
./core/src/main.cpp
./core/src/material.cpp
./core/src/materialparser.cpp 
./core/src/matrix.cpp
./core/src/matrix3.cpp
./core/src/md3model.cpp
./core/src/mesh.cpp
./core/src/meshloader3ds.cpp
./core/src/model.cpp
./core/src/modelmanager.cpp
./core/src/myglu.c
./core/src/openglext.cpp
./core/src/particlesystem.cpp
./core/src/physicsenvironment.cpp
./core/src/plane.cpp
./core/src/polygon.cpp
./core/src/quaternion.cpp
./core/src/renderable.cpp
./core/src/renderablemultimesh.cpp
./core/src/renderablequeuemember.cpp
./core/src/renderchunk.cpp
./core/src/renderer.cpp
./core/src/rendererARB2.cpp
./core/src/renderingview.cpp
./core/src/renderqueue.cpp
./core/src/rendersystem.cpp
./core/src/rendertexture.cpp
./core/src/resource.cpp
./core/src/resourcemanager.cpp
./core/src/rigidbody.cpp
./core/src/rigidbodystatic.cpp
./core/src/rigidbox.cpp #Error starts here
./core/src/rigidcapsule.cpp
./core/src/rigidcomplex.cpp
./core/src/rigidcyl.cpp
./core/src/rigidsphere.cpp #Error ends here
./core/src/scenemanager.cpp
./core/src/scenemd3.cpp
./core/src/scenemodel.cpp
./core/src/scenenode.cpp
./core/src/scenenodemember.cpp
./core/src/sceneobject.cpp
./core/src/shadowvolume.cpp
./core/src/silhouette.cpp
./core/src/skelanimation.cpp
./core/src/skeleton.cpp
./core/src/skeletoninstance.cpp
./core/src/skelmodel.cpp
./core/src/smfmodel.cpp
./core/src/splineinterpolator.cpp
./core/src/sylphis.rc
./core/src/texture.cpp
./core/src/texturefont.cpp
./core/src/timer.cpp
./core/src/tokenizer.cpp
./core/src/vector.cpp
./core/src/vertexbuffer.cpp)

SET(ZLIB_SOURCE
${ZLIB_SOURCE}
./hklib/extern/zlib/adler32.c
./hklib/extern/zlib/compress.c
./hklib/extern/zlib/crc32.c
./hklib/extern/zlib/deflate.c
./hklib/extern/zlib/gzio.c
./hklib/extern/zlib/infblock.c
./hklib/extern/zlib/infcodes.c
./hklib/extern/zlib/inffast.c
./hklib/extern/zlib/inflate.c
./hklib/extern/zlib/inftrees.c
./hklib/extern/zlib/infutil.c
./hklib/extern/zlib/maketree.c
./hklib/extern/zlib/trees.c
./hklib/extern/zlib/uncompr.c
./hklib/extern/zlib/unzip.cpp
./hklib/extern/zlib/zutil.c)

SET(PYTHON_SOURCE
${PYTHON_SOURCE}
./core/src/python/mthread.cpp
./core/src/python/pyconsole.cpp
./core/src/python/pyhelper.cpp
./core/src/python/sylphis_wrap.cpp
./core/src/python/waitable.cpp)

SET(LIBPNG_SOURCE
${LIBPNG_SOURCE}
./extern/libpng/example.c
./extern/libpng/png.c
./extern/libpng/pngerror.c
./extern/libpng/pnggccrd.c
./extern/libpng/pngget.c
./extern/libpng/pngmem.c
./extern/libpng/pngpread.c
./extern/libpng/pngread.c
./extern/libpng/pngrio.c
./extern/libpng/pngrtran.c
./extern/libpng/pngrutil.c
./extern/libpng/pngset.c
./extern/libpng/pngtest.c
./extern/libpng/pngtrans.c
./extern/libpng/pngvcrd.c
./extern/libpng/pngwio.c
./extern/libpng/pngwrite.c
./extern/libpng/pngwtran.c
./extern/libpng/pngwutil.c)

SET(CMAPSCENE_SOURCE
${CMAPSCENE_SOURCE}
./core/src/cmapscene/brushmodel.cpp
./core/src/cmapscene/cmap.cpp
./core/src/cmapscene/cmapscene.cpp # <- Talk to harkal about CVector3 references
)

SET(SOUND_SOURCE
${SOUND_SOURCE}
./core/src/sound/soundbuffer.cpp
./core/src/sound/soundenv.cpp
./core/src/sound/soundlistener.cpp
./core/src/sound/soundobject.cpp
./core/src/sound/soundogg.cpp
./core/src/sound/soundsource.cpp
./core/src/sound/soundwave.cpp)

SET(ALL_SOURCES
${ALL_SOURCES}
${SYLPHIS_CORE_SOURCE}
${ZLIB_SOURCE}
${PYTHON_SOURCE}
${LIBPNG_SOURCE}
${CMAPSCENE_SOURCE}
${SOUND_SOURCE})

#SET(SWIG_INTERFACE_FILE ./core/src/python/sylphis.i)
#SET(SWIG_OUTPUT_FILE ./core/src/python/sylphis_wrap.cpp) 

#SET_SOURCE_FILES_PROPERTIES(${SWIG_INTERFACE_FILE} PROPERTIES CPLUSPLUS ON)
#SET(SWIG_FLAGS "-o ${SWIG_OUTPUT_FILE}")
#SWIG_ADD_MODULE(sylphis_swig Python ${SWIG_INTERFACE_FILE})
#SWIG_LINK_LIBRARIES(sylphis_swig ${PYTHON_LIBRARIES})

ADD_EXECUTABLE(sylphis ${ALL_SOURCES})
TARGET_LINK_LIBRARIES(sylphis ${SYLPHIS_LIBRARIES})

#Make sure the swig generated file is generated before compiling sylphis
#ADD_DEPENDENCIES(sylphis sylphis_swig)

