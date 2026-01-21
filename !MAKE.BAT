@echo off
wcl386.exe /c /mf imguirec.cpp  -fo_imguirc.obj
wcl386.exe /c /mf imgui.cpp  -fo_imgui.obj
wcl386.exe /c /mf imguigl.cpp  -fo_imguigl.obj
wcl386.exe /c /mf glimpl.cpp  -fo_glimpl.obj
wcl386.exe /c /mf main.cpp  -fo_main.obj
wcl386.exe /c /mf glega.cpp  -fo_glega.obj
wcl386.exe /c /mf OBJECT.CPP -fo_object.obj
wcl386.exe /xr /c /mf STRING.CPP -fo_string.obj
wlink @DREAMER.LNK OPTION QUIET OPTION STack = 65536 NAME DREAMER.EXE
del _imgui.obj
del _imguigl.obj
del _glimpl.obj
del _object.obj
del _string.obj
del _main.obj
del _glega.obj
