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
	shader.h \
	shader.c \
	imgui_internal.h \
	imgui_impl_sdl_gl3.h \
	imgui_impl_sdl_gl3.cpp \
	$(abs_top_srcdir)/contrib/gl-matrix.c/vec3.c \
	$(abs_top_srcdir)/contrib/gl-matrix.c/mat3.c \
	$(abs_top_srcdir)/contrib/gl-matrix.c/mat4.c \
	$(abs_top_srcdir)/contrib/gl-matrix.c/quat.c \
	$(abs_top_srcdir)/contrib/gl-matrix.c/str.c \
	$(abs_top_srcdir)/contrib/imgui/imgui.cpp \
	$(abs_top_srcdir)/contrib/imgui/imgui_demo.cpp \
	$(abs_top_srcdir)/contrib/imgui/imgui_draw.cpp 
hurtboxdir = $(datarootdir)/$(PACKAGE)/
hurtbox_DATA = gamecontrollerdb.txt ../textures/thegrid.png
hurtbox_LDADD = $(LDFLAGS) -lSDL2_image
