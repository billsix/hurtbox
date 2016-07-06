AC_INIT([hurtbox], [0.0.1],       [William Emerison Six <billsix@gmail.com>], [hurtbox])
AC_CANONICAL_SYSTEM
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADER([config.h])
AC_CHECK_HEADER([stdio.h])
AC_REVISION([: 0.0.1 $])
AM_INIT_AUTOMAKE([subdir-objects])
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_CONFIG_FILES([Makefile doc/Makefile src/Makefile])

case "$target_os" in
    darwin*)
             LDFLAGS="-framework OpenGL $LDFLAGS"
             ;;
          *) LIBS="-lGLU $LIBS"
             ;;
esac

dnl Check for SDL
SDL_VERSION=2.0
AM_PATH_SDL2($SDL_VERSION,
            :,
            AC_MSG_ERROR([*** SDL version $SDL_VERSION not found!])
)
CFLAGS="$CFLAGS -std=c99 $SDL_CFLAGS"
CXXFLAGS="$CXXFLAGS $SDL_CFLAGS"
LIBS="$LIBS $SDL_LIBS"
AC_OUTPUT
