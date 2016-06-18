AC_INIT([fighting], [0.0.1],       [William Emerison Six <billsix@gmail.com>], [fighting])
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADER([config.h])
AC_CHECK_HEADER([stdio.h])
AC_REVISION([: 0.0.1 $])
AM_INIT_AUTOMAKE
AC_PROG_CC
AC_PROG_INSTALL
AC_CONFIG_FILES([Makefile doc/Makefile src/Makefile])

dnl Check for SDL
SDL_VERSION=2.0
AM_PATH_SDL2($SDL_VERSION,
            :,
            AC_MSG_ERROR([*** SDL version $SDL_VERSION not found!])
)
CFLAGS="$CFLAGS $SDL_CFLAGS"
LIBS="$LIBS $SDL_LIBS"
AC_OUTPUT
