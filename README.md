# ElfViewer

         ________  ___       ________  __   ___  ___  ________  __       ___  ________  ________
        /  _____/ /  /      /  _____/ |  | /  / /  / /  _____/ |  | _   /  / /  _____/ /  __   /
       /  /__    /  /      /  /__     |  |/  / /  / /  /__     |  |/ | /  / /  /__    /  /_/  /
      /  ___/   /  /      /   __/     |  /  / /  / /  ___/     |  /  |/  / /  ___/   /     _ /
     /  /____  /  /____  /  /         |    / /  / /  /____     |   _    / /  /____  /  / \  \ 
    /_______/ /_______/ /__/          |___/ /__/ /_______/     |__/ |__/ /_______/ /__/   \__\

A plugin for Total Commander to view meta data in ELF formatted files.

The ELF format is a standard file format used by many computer systems.
[https://en.wikipedia.org/wiki/Executable_and_Linkable_Format](https://en.wikipedia.org/wiki/Executable_and_Linkable_Format)

The plugin is based around the ELFIO library from
[http://elfio.sourceforge.net/](http://elfio.sourceforge.net/)

# Releases
Find the ready to use plugin under [releases](releases)
The release contains both 32 and 64 bit versions.

History:

1.0 - First public release ([ElfViewer_v1.0.zip](releases/ElfViewer_v1.0.zip))

## build your own

The plugin is build using mingw compiler
[https://sourceforge.net/projects/mingw-w64/](https://sourceforge.net/projects/mingw-w64/)

To build the plugin use below command.
```
mingw32-make
```
This will build 4 targets: debug, debug64, release, release64

To build only one of the targets use:
```
mingw32-make <target>
ex: mingw32-make debug
```
