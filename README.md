HurtBox is a playground for graphics programming.
It demostrates basic OpenGL rendering, with a camera moving
around a 3D world.  Includes intuitive GUI using nuklear.


Building
========

Windows VS2019
--------------

    build.bat


Linux/macOS
-----------

Prereqs

* git

* glfw3

* glfw3 development libraries

* gcc

Build

* using cmake

```bash
#Make a build directory
mkdir $(pwd)/../debug
#Make an install directory
mkdir $(pwd)/../debugInstall
export INSTALL_DIR=$(pwd)/../debugInstall
#Charge to the build directory
cd ../debug
CFLAGS='-g -O0' cmake ../hurtbox
cmake -DCMAKE_INSTALL_PREFIX:PATH=$INSTALL_DIR . && make all install
```