@echo off
if "%1"=="" goto pp
%COMMANDER_EXE% /i=%1 %2
goto exit_

:pp
echo "%0 <wincmd.ini file>"
echo.
:exit_