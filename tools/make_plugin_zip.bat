::@echo off

:: needs 7z tool
set z7z=c:\tools\7z
set out=ElfViewer_v1.0.zip

%z7z%\7za.exe a -tzip %out% @tc_plugin_zip_contents\filelist.txt

