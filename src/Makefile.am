bin_PROGRAMS = hurtbox
AM_CPPFLAGS = -I../contrib/imgui/ \
	-I../contrib/imgui/examples/sdl_opengl_example/ \
	$(shell sdl2-config --cflags)
hurtbox_SOURCES = \
	main.c \
	hb-imgui.cpp \
	../contrib/imgui/imgui.cpp \
	../contrib/imgui/imgui_demo.cpp \
	../contrib/imgui/imgui_draw.cpp \
	../contrib/imgui/imgui_internal.h \
	../contrib/imgui/examples/sdl_opengl_example/imgui_impl_sdl.cpp
hurtbox_LDADD = -lGL
