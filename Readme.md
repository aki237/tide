# Tide : Cleaner than Vim :P

I have implemented a text editor in Go..... But apparently Gtk+ hates the Go GC.
So decided to re write the whole project in C++ but using FLTK (Cross platform).
I think I have documented the code to some extent. Currently makefile is written for
Linux. Should work in unix like platforms using X11.

## Requirements

Following libraries and tools are required for building tide and it's dependancies.

#### Tools

+ cmake
+ GNUmake
+ Any working compiler toolchain supporting C++ 14 standard
  - GNU Compiler Collection (gcc) > 4.9
  - Clang Compiler (clang) > 3.4
  
#### Libraries

+ autotools
+ autotools additional macros for X11 and others.
+ Xlib, Xcb, Xft, Xcursor, Xinerama, Xext, Xau, Xdmcp, libgraphite libs and headers
+ FreeType, Harfbuzz.
+ libstdc++
+ Cairo

If you are using any kind of GUI in X11 most of these libraries must be installed already in your system.
Just install the `devel` packages of those packages from your distro.

## Building

Clone the source, update submodules and build the source as following.
```shell
git clone https://github.com/aki237/tide.git
cd tide
git submodule init
git submodule update
make
```
This will pull all the dependancies from the git repo sources and build them.
Following are built during the process:

+ [libclipboard](https://github.com/jtanx/libclipboard) - For Cross platform Clipboard purposes
+ [lua](https://lua.org) - Lua Embeddable Scripting library
+ [sol2](https://github.com/ThePhD/sol2/) - The Direct C++ to Lua binding.
+ [fltk](https://www.fltk.org) - Cross Platform Fast and Light Tool Kit

and the needed static libraries will be installed `install` directory in the root.

**For documentation run this**

```shell
make doc
```

**For cleaning the source directory.**
*Note* : This also cleans the doc directory.

```shell
make clean
```

All the docs will be stored in the docs directory. Every class (.cpp,.hpp) have been mentioned in the docs.
Currently the editor doesn't do much than moving around, typing, selection, line number, line highlight. etc.,
(only keyboard sorry because the text editing component is wholly written from scratch in Cairo Vector Drawing.)
Soon this will evolve into a normal notepad with all necessary utils (~~Save~~, ~~Copy~~, ~~Paste~~, Find&Replace).
I have already tested a lua embeding (Why Lua ? : lightweight) for this with which plugins can be written.
Any thoughts and help is welcome. Currently It only consumes only a little under 2 Megs of RAM.

*Lua plugin system is not yet ready. It is added for just demo purposes. The function calls may change in the future.*
