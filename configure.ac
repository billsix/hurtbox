AC_INIT([hurtbox], [0.0.1],       [William Emerison Six <billsix@gmail.com>], [hurtbox])
AC_CANONICAL_SYSTEM
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADER([config.h])
AC_CHECK_HEADER([stdio.h])
AC_REVISION([: 0.0.1 $])
AM_INIT_AUTOMAKE()
LT_PREREQ([2.4.2])
LT_INIT([disable-shared])
AC_PROG_CC
AC_PROG_CXX
AC_PROG_INSTALL
AC_CONFIG_FILES([Makefile \
        contrib/Makefile \
        doc/Makefile \
        src/Makefile])

case "$target_os" in
    darwin*)
             LDFLAGS="-framework OpenGL $LDFLAGS"
             OPENGL_LIB=""
             ;;
    ming*)
             OPENGL_LIB="-lopengl32"
             ;;
    *)
             OPENGL_LIB="-lGL"
             ;;

esac
AC_SUBST(OPENGL_LIB)

PKG_CHECK_MODULES(GLFW, glfw3 >= 3.2)

AC_OUTPUT
