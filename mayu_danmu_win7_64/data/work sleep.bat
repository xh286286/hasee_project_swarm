copy  D:\git_qt\build-sisvisit-Desktop_Qt_5_2_1_MSVC2010_32bit_OpenGL-Release\release\sisvisit.exe  .\
copy  D:\git_qt\sisvisit\*.ini .\data\
del .\data\proxy.ini

pause

sisvisit.exe workall

RUNDLL32.EXE PowrProf.dll,SetSuspendState