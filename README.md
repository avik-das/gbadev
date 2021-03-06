GBA Development
===============

A collection of programs written for the Nintendo GameBoy Advance (GBA). While larger games are possible, this repository will only contain smaller demo programs. Thus, it makes sense to keep them all in the same repository.

The included programs are described briefly below, while more elaborate descriptions are provided inside the corresponding directories.

Pre-requisites
--------------

The programs are mostly written in C, dropping down to ARM assembly if necessary. C++ is also an option, though it's not used here. The [devkitPro](https://devkitpro.org/) toolchain is used. The Makefiles assume that the toolchain has been installed and the `DEVKITARM` environment variable set up correctly.

Images are typically processed using the [GBA Raster Image Transmogrifier (Grit)](https://www.coranac.com/projects/grit/), so that tool also needs to be installed and available on the `PATH`. Due to the age of the grit project, installing it on a non-Windows machine is non-trivial. The next section details how to install it.

To run the program after compilation, a GBA emulator is required. Below is a section outlining possible suitable emulators.

Building and installing Grit
----------------------------

If developing on Windows, there are binaries already available. On other platforms, you will need to build grit yourself.

1. First, build and install [the FreeImage library](https://sourceforge.net/projects/freeimage/files/). The caveat is that you want a version of the library from around when the Grit software was last updated. For example, at the time of writing, the latest version of the grit source code is from 2010-03-27, so the appropriate version of FreeImage is 3.13.1 from 2009-12-22.

   Building FreeImage with a modern version of GCC required some minor changes to the source code. After that, the correct Makefile could be used to build the library:

   ```sh
   cd /path/to/downloaded/FreeImage

   # For Linux
   make -f Makefile.fip
   ```

1. Copy over the static library generated by the FreeImage build into the grit source directory. The caveat here is that FreeImage outputs `libfreeimageplus.a`, whereas grit expects `libfreeimage.a`. Copy it over with the changed name:

   ```sh
   cd /path/to/downloaded/grit_src
   cp /path/to/downloaded/FreeImage/Dist/libfreeimageplus.a libfreeimage.a
   ```

1. Now, you should be able to build grit using the provided Makefile:

   ```sh
   make
   ```

1. This will generate a `grit` binary in the current directory. This binary is statically linked, so just copy it over to somewhere on your `PATH` (or update your `PATH` to include the current directory). You can test that grit is successfully installed:

   ```sh
   # This should return the file path of the grit executable
   which grit
   ```

Emulator Compatibility
----------------------

The programs are mainly tested on one emulator:

* [**VisualBoy Advance**](http://vba-m.com/). The VBA-M fork is preferred, as the original project is now inactive. This emulator is very popular, so it makes sense to ensure compatibility.

Additionally, there are some other emulators to note:

* [**mednafen**](http://mednafen.sourceforge.net/). This emulator intends to emulate multiple platforms. These programs may not be tested extensively on this emulator, but it remains an option, and the programs may occasionally be tested on it.

* [**GBA.js**](https://endrift.github.io/gbajs/). This emulator runs in the browser using HTML5 and Javascript. The programs may not be tested extensively on this, but it has its place as an additional check.

Included programs
-----------------

More detailed descriptions are available in the README files inside the directory containing each program.

* [**sprite**](sprite). A simple test program that demonstrates a wide variety of hardware-provided functionality and general programming techniques.
