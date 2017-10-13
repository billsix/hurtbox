HurtBox is a playground for graphics programming.
It demostrates basic OpenGL rendering, with a camera moving
around a 3D world.  Includes intuitive GUI using nuklear.


Building
========

Linux/macOS/Windows (via Cygwin or MSys2)
-----------------------------------------

Prereqs

-git
-glfw3
-glfw3 development libraries
-gcc
-g++
-automake
-autoconf


$ ./autogen.sh
$ ./configure --prefix=$MY_INSTALL_DIR
$ make
$ make install


Visual Studio Community 2017
----------------------------

Prereqs

Nuget handles the depedencies
