set MVS_HOME="C:\Program Files\Microsoft Visual Studio 8"
set LIB=%MVS_HOME%\"VC\lib"
%MVS_HOME%\"VC\bin\cl" /c /I%JAVA_HOME%\include /I%JAVA_HOME%\include\win32 /I.\include/opencore-amrnb /I.\include/opencore-amrwb .\src\main\C\*.c .\src\main\C\list\*.c 
%MVS_HOME%\"VC\bin\link" /DLL /OUT:./res/libs/windows/libamr32.dll *.obj .\lib\C\windows\32\libopencore-amrnb.lib .\lib\C\windows\32\libopencore-amrwb.lib
del *.obj