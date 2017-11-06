HurtBox is a playground for graphics programming.
It demostrates basic OpenGL rendering, with a camera moving
around a 3D world.  Includes intuitive GUI using nuklear.


Building
========

Linux/macOS/Windows (via Cygwin or MSys2)
-----------------------------------------

Prereqs

* git

*glfw3

*glfw3 development libraries

*gcc

*cmake

    #Make a build directory
    mkdir $(pwd)/../debug
    #Make an install directory
    mkdir $(pwd)/../debugInstall
    export INSTALL_DIR=$(pwd)/../debugInstall
    #Charge to the build directory
    cd ../debug
    cmake ../hurtbox
    cmake -DCMAKE_INSTALL_PREFIX:PATH=$INSTALL_DIR . && make all install


Visual Studio Community 2017
----------------------------

Prereqs

Nuget handles the depedencies