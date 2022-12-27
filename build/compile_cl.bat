@ECHO OFF

cls

del *.obj *.pdb *.ilk

cl /nologo /Zi /GR- /EHs /W3 /MDd -D_CRT_SECURE_NO_WARNINGS /c -I ..\include -I ..\deps\esat\include -I ..\deps\nfd\include  ..\src\*.cc
cl /nologo /Zi /GR- /EHs /W3 /MDd /Fe:test.exe *.obj ..\deps\esat\bin\ESAT_d.lib ..\deps\nfd\bin\debug\nfd_d.lib  opengl32.lib user32.lib gdi32.lib shell32.lib Ole32.lib

copy .\test.exe ..\bin\
