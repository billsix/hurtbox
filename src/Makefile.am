EXTRA_DIST = gamecontrollerdb.txt

bin_PROGRAMS = hurtbox
AM_CPPFLAGS = \
	-I$(abs_top_srcdir)/contrib/gl-matrix-c/ \
	-I$(abs_top_srcdir)/contrib/nuklear/ \
	-D RESOURCE_DIR=\"$(hurtbox_dir)/\" \
	-D TEXTURE_DIR=\"$(hurtbox_texture_dir)/\" \
	-D SHADER_DIR=\"$(hurtbox_shader_dir)/\" \
	-D MODELS_DIR=\"$(hurtbox_model_dir)/\"

hurtbox_HEADERS = \
	common.h  \
	controller.h \
	intro-scene.h \
	main.h \
	mainscene.h \
	load_asset.h \
	shader.h

hurtbox_SOURCES = \
	controller.c \
	main.c \
	mainscene.c \
	shader.c \
	load_asset.c \
	intro-scene.c

hurtboxdir = $(datarootdir)/$(PACKAGE)/

hurtbox_DATA = gamecontrollerdb.txt

hurtbox_dir = $(pkgdatadir)
hurtbox_texture_dir = $(hurtbox_dir)/textures
hurtbox_texture__DATA = ../textures/thegrid.png

hurtbox_shader_dir = $(hurtbox_dir)/shaders
hurtbox_shader__DATA = \
	../shaders/walls.vert \
	../shaders/walls.frag

hurtbox_model_dir = $(hurtbox_dir)/models/
hurtbox_model__DATA = \
	../models/walls.blend


hurtbox_CFLAGS = \
	-std=c99 \
	$(GLEW_CFLAGS) \
	$(SDL_CFLAGS) \
	-I../contrib/nuklear/

hurtbox_CXXFLAGS= \
	$(GLEW_CFLAGS) \
	$(SDL_CFLAGS)

hurtbox_LDADD = \
	$(GLEW_LIBS) \
	$(SDL_LIBS) \
	$(OPENGL_LIB) \
	-lSDL2_image \
	../contrib/gl-matrix-c/libglmatrix.la \
	-lassimp \
	-lm
