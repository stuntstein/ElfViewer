@echo off
set file1=%TEMP%\~source~%~nx1.txt
set file2=%TEMP%\~target~%~nx2.txt
set wlx=tcelfviewer.wlx

echo %file1%
echo %file2%

wlx2txt.exe %wlx% %1 %file1% 
wlx2txt.exe %wlx% %2 %file2%
%COMMANDER_EXE% /O /S=C %file1% %file2%
del /q /f %file1%
del /q /f %file2%
