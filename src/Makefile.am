EXTRA_DIST = gamecontrollerdb.txt

bin_PROGRAMS = hurtbox
AM_CPPFLAGS = \
	-I$(abs_top_srcdir)/contrib/imgui/ \
	-I$(abs_top_srcdir)/contrib/imgui/examples/sdl_opengl_example/ \
	-I$(abs_top_srcdir)/contrib/gl-matrix.c/
hurtbox_SOURCES = \
	main.c \
	hb-imgui.cpp \
	mainscene.c \
	imgui_internal.h \
	$(abs_top_srcdir)/contrib/gl-matrix.c/vec3.c \
	$(abs_top_srcdir)/contrib/gl-matrix.c/mat3.c \
	$(abs_top_srcdir)/contrib/gl-matrix.c/mat4.c \
	$(abs_top_srcdir)/contrib/gl-matrix.c/quat.c \
	$(abs_top_srcdir)/contrib/gl-matrix.c/str.c \
	$(abs_top_srcdir)/contrib/imgui/imgui.cpp \
	$(abs_top_srcdir)/contrib/imgui/imgui_demo.cpp \
	$(abs_top_srcdir)/contrib/imgui/imgui_draw.cpp \
	$(abs_top_srcdir)/contrib/imgui/examples/sdl_opengl_example/imgui_impl_sdl.cpp
hurtboxdir = $(datarootdir)/$(PACKAGE)/
hurtbox_DATA = gamecontrollerdb.txt
hurtbox_LDADD = -lGL  -lGLEW

