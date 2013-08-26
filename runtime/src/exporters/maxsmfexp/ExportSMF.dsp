# Microsoft Developer Studio Project File - Name="ExportSMF" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=ExportSMF - Win32 Max 2x Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "ExportSMF.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "ExportSMF.mak" CFG="ExportSMF - Win32 Max 2x Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "ExportSMF - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "ExportSMF - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE "ExportSMF - Win32 Hybrid" (based on "Win32 (x86) Application")
!MESSAGE "ExportSMF - Win32 Max 2x Debug" (based on "Win32 (x86) Application")
!MESSAGE "ExportSMF - Win32 Max 2x Release" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "ExportSMF - Win32 Release"

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
# ADD CPP /nologo /GB /MD /W3 /O2 /I "C:\SDKs\Maxsdk\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib core.lib geom.lib gfx.lib mesh.lib maxutil.lib maxscrpt.lib paramblk2.lib /nologo /base:"0x105b0000" /subsystem:windows /dll /machine:I386 /out:"C:\Apps\3dsmax\Plugins\ExportSMF.dle" /libpath:"C:\SDKs\Maxsdk\lib" /release

!ELSEIF  "$(CFG)" == "ExportSMF - Win32 Debug"

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
# ADD CPP /nologo /GB /MDd /W3 /Gm /ZI /Od /I "C:\SDKs\Maxsdk\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib core.lib geom.lib gfx.lib mesh.lib maxutil.lib maxscrpt.lib paramblk2.lib /nologo /base:"0x105b0000" /subsystem:windows /dll /debug /machine:I386 /out:"C:\Apps\3dsmax\Plugins\ExportSMF.dle" /pdbtype:sept /libpath:"C:\SDKs\Maxsdk\lib"

!ELSEIF  "$(CFG)" == "ExportSMF - Win32 Hybrid"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ExportSMF___Win32_Hybrid"
# PROP BASE Intermediate_Dir "ExportSMF___Win32_Hybrid"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "ExportSMF___Win32_Hybrid"
# PROP Intermediate_Dir "ExportSMF___Win32_Hybrid"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /GZ /c
# ADD CPP /nologo /GB /MD /W3 /Gm /ZI /Od /I "C:\SDKs\Maxsdk\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib core.lib geom.lib gfx.lib mesh.lib maxutil.lib maxscrpt.lib paramblk2.lib /nologo /base:"0x105b0000" /subsystem:windows /dll /debug /machine:I386 /out:"C:\Apps\3dsmax\Plugins\ExportSMF.dle" /pdbtype:sept /libpath:"C:\SDKs\Maxsdk\lib"

!ELSEIF  "$(CFG)" == "ExportSMF - Win32 Max 2x Debug"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "ExportSMF___Win32_Max_2x_Debug"
# PROP BASE Intermediate_Dir "ExportSMF___Win32_Max_2x_Debug"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "ExportSMF___Win32_Max_2x_Debug"
# PROP Intermediate_Dir "ExportSMF___Win32_Max_2x_Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MDd /W3 /Gm /ZI /Od /I "C:\SDKs\Maxsdk\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /GB /MDd /W3 /Gm /ZI /Od /I "C:\SDKs\Max2sdk\include" /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_MAX2" /YX /FD /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "_DEBUG"
# ADD RSC /l 0x409 /d "_DEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib core.lib geom.lib gfx.lib mesh.lib maxutil.lib maxscrpt.lib paramblk2.lib /nologo /base:"0x105b0000" /subsystem:windows /dll /debug /machine:I386 /out:"C:\Apps\3dsmax\Plugins\ExportSMF.dle" /pdbtype:sept /libpath:"C:\SDKs\Maxsdk\lib"
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib core.lib geom.lib gfx.lib mesh.lib /nologo /base:"0x105b0000" /subsystem:windows /dll /debug /machine:I386 /out:"C:\Apps\3D Studio Max R2\Plugins\ExportSMF.dle" /pdbtype:sept /libpath:"C:\SDKs\Max2sdk\lib"

!ELSEIF  "$(CFG)" == "ExportSMF - Win32 Max 2x Release"

# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "ExportSMF___Win32_Max_2x_Release"
# PROP BASE Intermediate_Dir "ExportSMF___Win32_Max_2x_Release"
# PROP BASE Ignore_Export_Lib 0
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "ExportSMF___Win32_Max_2x_Release"
# PROP Intermediate_Dir "ExportSMF___Win32_Max_2x_Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /G6 /MD /W3 /O2 /I "C:\SDKs\Maxsdk\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /FD /c
# ADD CPP /nologo /GB /MD /W3 /O2 /I "C:\SDKs\Max2sdk\include" /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MAX2" /YX /FD /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD BASE RSC /l 0x409 /d "NDEBUG"
# ADD RSC /l 0x409 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib core.lib geom.lib gfx.lib mesh.lib maxutil.lib maxscrpt.lib paramblk2.lib /nologo /base:"0x105b0000" /subsystem:windows /dll /machine:I386 /out:"C:\Apps\3dsmax\Plugins\ExportSMF.dle" /libpath:"C:\SDKs\Maxsdk\lib" /release
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib comctl32.lib core.lib geom.lib gfx.lib mesh.lib /nologo /base:"0x105b0000" /subsystem:windows /dll /machine:I386 /out:"C:\Apps\3D Studio Max R2\Plugins\ExportSMF.dle" /libpath:"C:\SDKs\Max2sdk\lib" /release

!ENDIF 

# Begin Target

# Name "ExportSMF - Win32 Release"
# Name "ExportSMF - Win32 Debug"
# Name "ExportSMF - Win32 Hybrid"
# Name "ExportSMF - Win32 Max 2x Debug"
# Name "ExportSMF - Win32 Max 2x Release"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\DiskIO.cpp
# End Source File
# Begin Source File

SOURCE=.\DllEntry.cpp
# End Source File
# Begin Source File

SOURCE=.\DoExportSMF.cpp
# End Source File
# Begin Source File

SOURCE=.\ExportSMF.cpp
# End Source File
# Begin Source File

SOURCE=.\ExportSMF.def
# End Source File
# Begin Source File

SOURCE=.\ExportSMF.rc
# End Source File
# End Group
# Begin Group "Header Files"

# PROP Default_Filter "h;hpp;hxx;hm;inl"
# Begin Source File

SOURCE=.\DiskIO.h
# End Source File
# Begin Source File

SOURCE=.\ExportSMF.h
# End Source File
# Begin Source File

SOURCE=.\resource.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# End Group
# End Target
# End Project
