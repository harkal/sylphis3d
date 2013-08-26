# Microsoft Developer Studio Project File - Name="sylphis" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=sylphis - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "sylphis.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "sylphis.mak" CFG="sylphis - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "sylphis - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "sylphis - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "sylphis - Win32 Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD CPP /nologo /G6 /W3 /Gi /GX /O2 /Ob2 /I "./src/" /I "include/" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x408 /d "NDEBUG"
# ADD RSC /l 0x408 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 ode.lib libjpeg.lib opengl32.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /profile /map /debug /machine:I386 /out:"e:\sylphis\sylphis.exe"

!ELSEIF  "$(CFG)" == "sylphis - Win32 Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /YX /FD /GZ /c
# ADD CPP /nologo /W3 /Gm /GX /ZI /Od /I "./src/" /I "include/" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MBCS" /FR /YX /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x408 /d "_DEBUG"
# ADD RSC /l 0x408 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 ode.lib libjpeg.lib opengl32.lib winmm.lib kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /profile /map /debug /machine:I386 /out:"e:\sylphis\sylphis.exe"

!ENDIF 

# Begin Target

# Name "sylphis - Win32 Release"
# Name "sylphis - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\src\actor.cpp
# End Source File
# Begin Source File

SOURCE=.\src\actormover.cpp
# End Source File
# Begin Source File

SOURCE=.\src\animationblender.cpp
# End Source File
# Begin Source File

SOURCE=.\src\bonekeyframe.cpp
# End Source File
# Begin Source File

SOURCE=.\src\bonetrack.cpp
# End Source File
# Begin Source File

SOURCE=.\src\boundbox.cpp
# End Source File
# Begin Source File

SOURCE=.\src\camera.cpp
# End Source File
# Begin Source File

SOURCE=.\src\client.cpp

!IF  "$(CFG)" == "sylphis - Win32 Release"

!ELSEIF  "$(CFG)" == "sylphis - Win32 Debug"

# ADD CPP /U "_DEBUG"
# SUBTRACT CPP /D "_DEBUG"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\commands.cpp
# End Source File
# Begin Source File

SOURCE=.\src\configfile.cpp
# End Source File
# Begin Source File

SOURCE=.\src\console.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cvars.cpp
# End Source File
# Begin Source File

SOURCE=.\src\engine.cpp

!IF  "$(CFG)" == "sylphis - Win32 Release"

!ELSEIF  "$(CFG)" == "sylphis - Win32 Debug"

# ADD CPP /U "_DEBUG"
# SUBTRACT CPP /D "_DEBUG"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\enginecomponent.cpp
# End Source File
# Begin Source File

SOURCE=.\src\eventlistener.cpp
# End Source File
# Begin Source File

SOURCE=.\src\exception.cpp
# End Source File
# Begin Source File

SOURCE=.\src\frustum.cpp
# End Source File
# Begin Source File

SOURCE=.\src\imagefile.cpp
# End Source File
# Begin Source File

SOURCE=.\src\inputdevice.cpp
# End Source File
# Begin Source File

SOURCE=.\src\interpolator.cpp
# End Source File
# Begin Source File

SOURCE=.\src\light.cpp
# End Source File
# Begin Source File

SOURCE=.\src\main.cpp
# End Source File
# Begin Source File

SOURCE=.\src\matrix.cpp
# End Source File
# Begin Source File

SOURCE=.\src\mesh.cpp
# End Source File
# Begin Source File

SOURCE=.\src\meshloader3ds.cpp
# End Source File
# Begin Source File

SOURCE=.\src\meshloadermd2.cpp
# End Source File
# Begin Source File

SOURCE=.\src\meshmanager.cpp
# End Source File
# Begin Source File

SOURCE=.\src\model.cpp
# End Source File
# Begin Source File

SOURCE=.\src\modelmanager.cpp
# End Source File
# Begin Source File

SOURCE=.\src\myglu.c
# End Source File
# Begin Source File

SOURCE=.\src\normalmap.cpp
# End Source File
# Begin Source File

SOURCE=.\src\openglext.cpp
# End Source File
# Begin Source File

SOURCE=.\src\plane.cpp
# End Source File
# Begin Source File

SOURCE=.\src\pointer.cpp
# End Source File
# Begin Source File

SOURCE=.\src\quaternion.cpp
# End Source File
# Begin Source File

SOURCE=.\src\refobject.cpp
# End Source File
# Begin Source File

SOURCE=.\src\renderable.cpp
# End Source File
# Begin Source File

SOURCE=.\src\renderablemd2.cpp
# End Source File
# Begin Source File

SOURCE=.\src\renderablemultimesh.cpp
# End Source File
# Begin Source File

SOURCE=.\src\renderablequeuemember.cpp
# End Source File
# Begin Source File

SOURCE=.\src\renderer.cpp
# End Source File
# Begin Source File

SOURCE=.\src\rendererGF3.cpp
# End Source File
# Begin Source File

SOURCE=.\src\rendererGL.cpp
# End Source File
# Begin Source File

SOURCE=.\src\renderingview.cpp
# End Source File
# Begin Source File

SOURCE=.\src\renderingviewwin32.cpp
# End Source File
# Begin Source File

SOURCE=.\src\renderqueue.cpp
# End Source File
# Begin Source File

SOURCE=.\src\resource.cpp
# End Source File
# Begin Source File

SOURCE=.\src\resourcemanager.cpp
# End Source File
# Begin Source File

SOURCE=.\src\rigidbody.cpp
# End Source File
# Begin Source File

SOURCE=.\src\scenemanager.cpp
# End Source File
# Begin Source File

SOURCE=.\src\scenemodel.cpp
# End Source File
# Begin Source File

SOURCE=.\src\scenenode.cpp
# End Source File
# Begin Source File

SOURCE=.\src\scenenodemember.cpp
# End Source File
# Begin Source File

SOURCE=.\src\sceneobject.cpp
# End Source File
# Begin Source File

SOURCE=.\src\server.cpp
# End Source File
# Begin Source File

SOURCE=.\src\serverclientbase.cpp
# End Source File
# Begin Source File

SOURCE=.\src\shader.cpp
# End Source File
# Begin Source File

SOURCE=.\src\silhouette.cpp
# End Source File
# Begin Source File

SOURCE=.\src\singleton.cpp
# End Source File
# Begin Source File

SOURCE=.\src\skelanimation.cpp
# End Source File
# Begin Source File

SOURCE=.\src\skeleton.cpp
# End Source File
# Begin Source File

SOURCE=.\src\skeletoninstance.cpp
# End Source File
# Begin Source File

SOURCE=.\src\texture.cpp
# End Source File
# Begin Source File

SOURCE=.\src\timer.cpp
# End Source File
# Begin Source File

SOURCE=.\src\tokenizer.cpp
# End Source File
# Begin Source File

SOURCE=.\src\vector.cpp
# End Source File
# Begin Source File

SOURCE=.\src\vfs.cpp
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\src\actor.h
# End Source File
# Begin Source File

SOURCE=.\src\actormover.h
# End Source File
# Begin Source File

SOURCE=.\src\animationblender.h
# End Source File
# Begin Source File

SOURCE=.\src\bonekeyframe.h
# End Source File
# Begin Source File

SOURCE=.\src\bonetrack.h
# End Source File
# Begin Source File

SOURCE=.\src\bound.h
# End Source File
# Begin Source File

SOURCE=.\src\boundbox.h
# End Source File
# Begin Source File

SOURCE=.\src\build.h
# End Source File
# Begin Source File

SOURCE=.\src\camera.h
# End Source File
# Begin Source File

SOURCE=.\src\client.h
# End Source File
# Begin Source File

SOURCE=.\src\commands.h
# End Source File
# Begin Source File

SOURCE=.\src\configfile.h
# End Source File
# Begin Source File

SOURCE=.\src\console.h
# End Source File
# Begin Source File

SOURCE=.\src\cvars.h
# End Source File
# Begin Source File

SOURCE=.\src\engine.h
# End Source File
# Begin Source File

SOURCE=.\src\enginecomponent.h
# End Source File
# Begin Source File

SOURCE=.\src\eventlistener.h
# End Source File
# Begin Source File

SOURCE=.\src\exception.h
# End Source File
# Begin Source File

SOURCE=.\src\frustum.h
# End Source File
# Begin Source File

SOURCE=.\src\globaldefs.h
# End Source File
# Begin Source File

SOURCE=.\src\hapdbg.h
# End Source File
# Begin Source File

SOURCE=.\src\imagefile.h
# End Source File
# Begin Source File

SOURCE=.\src\inputdevice.h
# End Source File
# Begin Source File

SOURCE=.\src\interpolator.h
# End Source File
# Begin Source File

SOURCE=.\src\jconfig.h
# End Source File
# Begin Source File

SOURCE=.\src\jmorecfg.h
# End Source File
# Begin Source File

SOURCE=.\src\light.h
# End Source File
# Begin Source File

SOURCE=.\src\matrix.h
# End Source File
# Begin Source File

SOURCE=.\src\mesh.h
# End Source File
# Begin Source File

SOURCE=.\src\meshloader3ds.h
# End Source File
# Begin Source File

SOURCE=.\src\meshloadermd2.h
# End Source File
# Begin Source File

SOURCE=.\src\meshmanager.h
# End Source File
# Begin Source File

SOURCE=.\src\model.h
# End Source File
# Begin Source File

SOURCE=.\src\modelmanager.h
# End Source File
# Begin Source File

SOURCE=.\src\myglu.h
# End Source File
# Begin Source File

SOURCE=.\src\normalmap.h
# End Source File
# Begin Source File

SOURCE=.\src\openglext.h
# End Source File
# Begin Source File

SOURCE=.\src\plane.h
# End Source File
# Begin Source File

SOURCE=.\src\pointer.h
# End Source File
# Begin Source File

SOURCE=.\src\quaternion.h
# End Source File
# Begin Source File

SOURCE=.\src\refobject.h
# End Source File
# Begin Source File

SOURCE=.\src\renderable.h
# End Source File
# Begin Source File

SOURCE=.\src\renderablemd2.h
# End Source File
# Begin Source File

SOURCE=.\src\renderablemultimesh.h
# End Source File
# Begin Source File

SOURCE=.\src\renderablequeuemember.h
# End Source File
# Begin Source File

SOURCE=.\src\renderer.h
# End Source File
# Begin Source File

SOURCE=.\src\rendererGF3.h
# End Source File
# Begin Source File

SOURCE=.\src\rendererGL.h
# End Source File
# Begin Source File

SOURCE=.\src\renderingview.h
# End Source File
# Begin Source File

SOURCE=.\src\renderingviewwin32.h
# End Source File
# Begin Source File

SOURCE=.\src\renderingviewx11.h
# End Source File
# Begin Source File

SOURCE=.\src\renderqueue.h
# End Source File
# Begin Source File

SOURCE=.\src\resource.h
# End Source File
# Begin Source File

SOURCE=.\src\resourcemanager.h
# End Source File
# Begin Source File

SOURCE=.\src\rigidbody.h
# End Source File
# Begin Source File

SOURCE=.\src\scenemanager.h
# End Source File
# Begin Source File

SOURCE=.\src\scenemodel.h
# End Source File
# Begin Source File

SOURCE=.\src\scenenode.h
# End Source File
# Begin Source File

SOURCE=.\src\scenenodemember.h
# End Source File
# Begin Source File

SOURCE=.\src\sceneobject.h
# End Source File
# Begin Source File

SOURCE=.\src\server.h
# End Source File
# Begin Source File

SOURCE=.\src\serverclientbase.h
# End Source File
# Begin Source File

SOURCE=.\src\shader.h
# End Source File
# Begin Source File

SOURCE=.\src\silhouette.h
# End Source File
# Begin Source File

SOURCE=.\src\singleton.h
# End Source File
# Begin Source File

SOURCE=.\src\skelanimation.h
# End Source File
# Begin Source File

SOURCE=.\src\skeleton.h
# End Source File
# Begin Source File

SOURCE=.\src\skeletoninstance.h
# End Source File
# Begin Source File

SOURCE=.\src\texture.h
# End Source File
# Begin Source File

SOURCE=.\src\timer.h
# End Source File
# Begin Source File

SOURCE=.\src\tokenizer.h
# End Source File
# Begin Source File

SOURCE=.\src\vector.h
# End Source File
# Begin Source File

SOURCE=.\src\vfs.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\src\sylphis.ico
# End Source File
# Begin Source File

SOURCE=.\src\sylphis.rc
# End Source File
# End Group
# Begin Group "zlib"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\zlib\adler32.c
# End Source File
# Begin Source File

SOURCE=.\src\zlib\compress.c
# End Source File
# Begin Source File

SOURCE=.\src\zlib\crc32.c
# End Source File
# Begin Source File

SOURCE=.\src\zlib\deflate.c
# End Source File
# Begin Source File

SOURCE=.\src\zlib\deflate.h
# End Source File
# Begin Source File

SOURCE=.\src\zlib\gzio.c
# End Source File
# Begin Source File

SOURCE=.\src\zlib\infblock.c
# End Source File
# Begin Source File

SOURCE=.\src\zlib\infblock.h
# End Source File
# Begin Source File

SOURCE=.\src\zlib\infcodes.c
# End Source File
# Begin Source File

SOURCE=.\src\zlib\infcodes.h
# End Source File
# Begin Source File

SOURCE=.\src\zlib\inffast.c
# End Source File
# Begin Source File

SOURCE=.\src\zlib\inffast.h
# End Source File
# Begin Source File

SOURCE=.\src\zlib\inffixed.h
# End Source File
# Begin Source File

SOURCE=.\src\zlib\inflate.c
# End Source File
# Begin Source File

SOURCE=.\src\zlib\inftrees.c
# End Source File
# Begin Source File

SOURCE=.\src\zlib\inftrees.h
# End Source File
# Begin Source File

SOURCE=.\src\zlib\infutil.c
# End Source File
# Begin Source File

SOURCE=.\src\zlib\infutil.h
# End Source File
# Begin Source File

SOURCE=.\src\zlib\maketree.c
# End Source File
# Begin Source File

SOURCE=.\src\zlib\trees.c
# End Source File
# Begin Source File

SOURCE=.\src\zlib\trees.h
# End Source File
# Begin Source File

SOURCE=.\src\zlib\uncompr.c
# End Source File
# Begin Source File

SOURCE=.\src\zlib\unzip.cpp
# End Source File
# Begin Source File

SOURCE=.\src\zlib\unzip.h
# End Source File
# Begin Source File

SOURCE=.\src\zlib\zconf.h
# End Source File
# Begin Source File

SOURCE=.\src\zlib\zlib.h
# End Source File
# Begin Source File

SOURCE=.\src\zlib\zutil.c
# End Source File
# Begin Source File

SOURCE=.\src\zlib\zutil.h
# End Source File
# End Group
# Begin Group "q3bspscene"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\q3bspscene\bsp.cpp
# End Source File
# Begin Source File

SOURCE=.\src\q3bspscene\bsp.h
# End Source File
# Begin Source File

SOURCE=.\src\q3bspscene\bspchunk.cpp
# End Source File
# Begin Source File

SOURCE=.\src\q3bspscene\bspchunk.h
# End Source File
# Begin Source File

SOURCE=.\src\q3bspscene\q3bspscene.cpp

!IF  "$(CFG)" == "sylphis - Win32 Release"

!ELSEIF  "$(CFG)" == "sylphis - Win32 Debug"

# ADD CPP /U "_DEBUG"
# SUBTRACT CPP /D "_DEBUG"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\q3bspscene\q3bspscene.h
# End Source File
# Begin Source File

SOURCE=.\src\q3bspscene\typedefs.h
# End Source File
# End Group
# Begin Group "python"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\python\initmodules.h
# End Source File
# Begin Source File

SOURCE=.\src\python\pyhelper.h
# End Source File
# Begin Source File

SOURCE=.\src\python\sylphis.i

!IF  "$(CFG)" == "sylphis - Win32 Release"

!ELSEIF  "$(CFG)" == "sylphis - Win32 Debug"

# Begin Custom Build
InputDir=.\src\python
InputPath=.\src\python\sylphis.i
InputName=sylphis

"$(InputDir)\$(InputName)_wrap.cpp" : $(SOURCE) "$(INTDIR)" "$(OUTDIR)"
	e:\python22\swig\swig -Wall -c++ -python -I..\ -o $(InputDir)\$(InputName)_wrap.cpp $(InputPath)

# End Custom Build

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\python\sylphis_wrap.cpp

!IF  "$(CFG)" == "sylphis - Win32 Release"

!ELSEIF  "$(CFG)" == "sylphis - Win32 Debug"

# ADD CPP /U "_DEBUG"
# SUBTRACT CPP /D "_DEBUG"

!ENDIF 

# End Source File
# End Group
# Begin Group "libpng"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\libpng\png.c
# End Source File
# Begin Source File

SOURCE=.\src\libpng\png.h
# End Source File
# Begin Source File

SOURCE=.\src\libpng\pngasmrd.h
# End Source File
# Begin Source File

SOURCE=.\src\libpng\pngconf.h
# End Source File
# Begin Source File

SOURCE=.\src\libpng\pngerror.c
# End Source File
# Begin Source File

SOURCE=.\src\libpng\pnggccrd.c
# End Source File
# Begin Source File

SOURCE=.\src\libpng\pngget.c
# End Source File
# Begin Source File

SOURCE=.\src\libpng\pngmem.c
# End Source File
# Begin Source File

SOURCE=.\src\libpng\pngpread.c
# End Source File
# Begin Source File

SOURCE=.\src\libpng\pngread.c
# End Source File
# Begin Source File

SOURCE=.\src\libpng\pngrio.c
# End Source File
# Begin Source File

SOURCE=.\src\libpng\pngrtran.c
# End Source File
# Begin Source File

SOURCE=.\src\libpng\pngrutil.c
# End Source File
# Begin Source File

SOURCE=.\src\libpng\pngset.c
# End Source File
# Begin Source File

SOURCE=.\src\libpng\pngtest.c
# End Source File
# Begin Source File

SOURCE=.\src\libpng\pngtrans.c
# End Source File
# Begin Source File

SOURCE=.\src\libpng\pngvcrd.c
# End Source File
# Begin Source File

SOURCE=.\src\libpng\pngwio.c
# End Source File
# Begin Source File

SOURCE=.\src\libpng\pngwrite.c
# End Source File
# Begin Source File

SOURCE=.\src\libpng\pngwtran.c
# End Source File
# Begin Source File

SOURCE=.\src\libpng\pngwutil.c
# End Source File
# End Group
# Begin Group "cmapscene"

# PROP Default_Filter ""
# Begin Source File

SOURCE=.\src\cmapscene\cmap.cpp
# End Source File
# Begin Source File

SOURCE=.\src\cmapscene\cmap.h
# End Source File
# Begin Source File

SOURCE=.\src\cmapscene\cmapscene.cpp

!IF  "$(CFG)" == "sylphis - Win32 Release"

!ELSEIF  "$(CFG)" == "sylphis - Win32 Debug"

# ADD CPP /U "_DEBUG"
# SUBTRACT CPP /D "_DEBUG"

!ENDIF 

# End Source File
# Begin Source File

SOURCE=.\src\cmapscene\cmapscene.h
# End Source File
# Begin Source File

SOURCE=.\src\cmapscene\typedefs.h
# End Source File
# End Group
# End Target
# End Project
