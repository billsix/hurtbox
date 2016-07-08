bin_PROGRAMS = hurtbox
AM_CPPFLAGS = \
	-I$(abs_top_srcdir)/contrib/imgui/ \
	-I$(abs_top_srcdir)/contrib/imgui/examples/sdl_opengl_example/
hurtbox_SOURCES = \
	main.c \
	controller.c \
	hb-imgui.cpp \
	imgui_internal.h \
	$(abs_top_srcdir)/contrib/imgui/imgui.cpp \
	$(abs_top_srcdir)/contrib/imgui/imgui_demo.cpp \
	$(abs_top_srcdir)/contrib/imgui/imgui_draw.cpp \
	$(abs_top_srcdir)/contrib/imgui/examples/sdl_opengl_example/imgui_impl_sdl.cpp
hurtbox_LDADD = -lGL 

