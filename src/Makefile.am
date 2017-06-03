EXTRA_DIST = gamecontrollerdb.txt

bin_PROGRAMS = hurtbox
AM_CPPFLAGS = \
	-I$(abs_top_srcdir)/contrib/gl-matrix.c/ \
	-D RESOURCE_DIR=\"$(hurtbox_dir)/\" \
	-D TEXTURE_DIR=\"$(hurtbox_texture_dir)/\" \
	-D SHADER_DIR=\"$(hurtbox_shader_dir)/\" 
hurtbox_SOURCES = \
	main.c \
	mainscene.c \
	shader.h \
	shader.c \
	intro-scene.c \
	intro-scene.h \
	$(abs_top_srcdir)/contrib/gl-matrix.c/vec3.c \
	$(abs_top_srcdir)/contrib/gl-matrix.c/mat3.c \
	$(abs_top_srcdir)/contrib/gl-matrix.c/mat4.c \
	$(abs_top_srcdir)/contrib/gl-matrix.c/quat.c \
	$(abs_top_srcdir)/contrib/gl-matrix.c/str.c 
hurtboxdir = $(datarootdir)/$(PACKAGE)/

hurtbox_DATA = gamecontrollerdb.txt

hurtbox_dir = $(pkgdatadir)
hurtbox_texture_dir = $(hurtbox_dir)/textures
hurtbox_texture__DATA = ../textures/thegrid.png

hurtbox_shader_dir = $(hurtbox_dir)/shaders
hurtbox_shader__DATA = \
	../shaders/walls.vert \
	../shaders/walls.frag 


hurtbox_CFLAGS = \
	-std=c99 \
	$(GLEW_CFLAGS) \
	$(SDL_CFLAGS)

hurtbox_CXXFLAGS= \
	$(GLEW_CFLAGS) \
	$(SDL_CFLAGS)

hurtbox_LDADD = \
	$(GLEW_LIBS) \
	$(SDL_LIBS) \
	$(OPENGL_LIB) \
	-lSDL2_image \
	-lm
