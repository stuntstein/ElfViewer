@echo off
set VOL=%CD:~0,1%
set VOL=c
if "%1"=="" goto pp
%VOL%:\totalcmd\totalcmd64.exe /i=%1 %2
goto exit

:pp
echo test.exe <wincmd.ini file>
echo.
:exit