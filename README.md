# Sonic Riders Tournament Edition

This repo contains a WIP decompilation of Sonic Riders Tournament Edition, which is based off of Sonic Riders (USA).

It builds the following DOL:

main.dol: `sha1: 1be22a929b4d51a11dcf1bee34d45600b561ee6c`

**NOTE:** The DOL is currently *shiftable*. This means you can add data anywhere you want, without it breaking the entire game.

## Building

### Required packages

* [devkitPro](https://devkitpro.org/wiki/Getting_Started)

Below are required MSYS2 terminal packages (in-depth installation is found in the [instructions](#instructions)):
* mingw-w64-x86_64-python
* mingw-w64-x86_64-cmake
* mingw-w64-x86_64-gcc
* dkp-cmake-common-utils

### Required external libraries

* [SRTE libOGC](https://github.com/Sonic-Riders-Tournament-Edition/libogc-srte) - TE source code is now compiled using a custom version of libOGC rather than the embedded Nintendo Dolphin SDK. Check the [build instructions](#instructions) for compiling this library.

### Instructions

**These instructions are catered to Windows. Instructions may vary on different operating systems.**

For maximum compatibility, you should install devkitPro using their graphical installer, and you should use the MSYS2 terminal included with your devkitPro installation, using the MINGW64 environment. By default, the MSYS2 terminal launches in the MSYS2 environment. To launch it in the MINGW64 environment, run `mingw64.exe` in your devkitPro's MSYS2 installation folder. The following instructions assume you are using the MSYS2 terminal with the proper environment.

1. Make sure your terminal is up to date. Do this via `pacman -Syu`.

2. Make sure you have Python installed in your environment. You can do so via `pacman -S mingw-w64-x86_64-python`.
    1. If this gives you an error saying `error: target not found`, you need to add all the mingw64 repositories to your pacman.
    2. To do this, run `nano /etc/pacman.conf` to open up pacman's configuration in a CLI text editor.
    3. Add the following lines at the bottom of this file:
    ```
    [mingw64]
    Include = /etc/pacman.d/mirrorlist.mingw64
    ```
    4. Press `CTRL+X`, then `Y`, then `Enter` to save changes.
    5. Run `pacman -Syu` to update your changes, then try installing the package again.

3. Make sure you have the correct version of cmake installed. You can do this in the terminal via `pacman -S mingw-w64-x86_64-cmake`.

4. Also make sure you have the following packages installed using these commands: `pacman -S mingw-w64-x86_64-gcc` and `pacman -S dkp-cmake-common-utils`.

5. Last but not least, navigate to your devkitPro folder (usually at `C:\devkitPro`), and go into the `cmake` folder and open `dkp-initialize-path.cmake` with a text editor.

6. Comment out lines 3-5 using the `#` symbol, so that they look like this:
```
#if(CMAKE_HOST_WIN32)
#	message(FATAL_ERROR "CMake must be installed and launched from msys2: pacman -S cmake")
#endif()
```

7. Once that's done, clone the aforementioned custom libOGC library to your computer via `git clone` or any other similar way. **(This step doesn't go through the MSYS2 terminal.)**

8. Once cloned, navigate to the cloned directory in the terminal via the `cd` command. Then run `make cuberiders` to build the library. Once it's built, make sure to note down the full absolute path to this cloned directory. You'll need it in the later steps.

9. Then, clone the source code repository to your computer. **(This step doesn't go through the MSYS2 terminal.)**

10. Once cloned, navigate to the `tools` directory in the terminal using the `cd` command. Then run the `makeTools.sh` script via this command: `./makeTools.sh`.

11. After that, make a new folder in the root folder of the project, where the build files will be generated in. For example, you can name the folder `build`.

12. Navigate into that directory using the `cd` command again. This is where you'll need the path to the libOGC library folder mentioned in step 8. Run the following command to generate the build files, replacing `/path/to/libogc` with your noted down path (NOTE: if on Windows, the path's `\` separators must be using `/` instead!): 
```
$DEVKITPRO/devkitPPC/bin/powerpc-eabi-cmake -DCUSTOM_LIBOGC_PATH=/path/to/libogc ../
```

13. Run `ninja` in the same directory to build the DOL file (you can also run `ninja -j$(nproc --all)` for faster build times). The `main.dol` file will be built and compiled into the same build folder you created.

## Contributions

Contributions and PRs are welcome.
