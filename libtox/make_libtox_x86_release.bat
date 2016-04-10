@echo libtox x86 release
@echo off

if exist "%~dp0..\libtox_x86_release" rd /q /s "%~dp0..\libtox_x86_release"

md "%~dp0..\libtox_x86_release"
md "%~dp0..\libtox_x86_release\bin"
md "%~dp0..\libtox_x86_release\include"
md "%~dp0..\libtox_x86_release\include\tox"
md "%~dp0..\libtox_x86_release\lib"

copy "%~dp0..\build\Release\libtox.dll" "%~dp0..\libtox_x86_release\bin"
copy "%~dp0..\build\Release\libtox.map" "%~dp0..\libtox_x86_release\bin"
copy "%~dp0..\build\Release\libtox.lib" "%~dp0..\libtox_x86_release\lib"
copy "%~dp0..\toxcore\toxcore\tox.h" "%~dp0..\libtox_x86_release\include\tox"
copy "%~dp0..\toxcore\toxcore\tox_old.h" "%~dp0..\libtox_x86_release\include\tox"
copy "%~dp0..\toxcore\toxdns\toxdns.h" "%~dp0..\libtox_x86_release\include\tox"
copy "%~dp0..\toxcore\toxencryptsave\toxencryptsave.h" "%~dp0..\libtox_x86_release\include\tox"
copy "%~dp0..\toxcore\toxav\toxav.h" "%~dp0..\libtox_x86_release\include\tox"

@echo libtox x86 release prepared
