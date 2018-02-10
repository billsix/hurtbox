bin_PROGRAMS = hurtbox
AM_CPPFLAGS = \
	-I$(abs_top_srcdir)/contrib/gl-matrix-stack/ \
	-I$(abs_top_srcdir)/contrib/nuklear/ \
	-I$(abs_top_srcdir)/contrib/gl3w-Single-File/ \
	-I$(abs_top_srcdir)/contrib/stb/ \
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

hurtbox_dir = $(pkgdatadir)
hurtbox_texture_dir = $(hurtbox_dir)/textures
hurtbox_texture__DATA = ../textures/thegrid.png

hurtbox_shader_dir = $(hurtbox_dir)/shaders
hurtbox_shader__DATA = \
	../shaders/walls.vert \
	../shaders/walls.frag

hurtbox_model_dir = $(hurtbox_dir)/models/
hurtbox_model__DATA = \
	../models/walls.dae \
	../models/walls.obj


hurtbox_CFLAGS = \
	-std=c99 \
	$(GLFW_CFLAGS) \
	$(ASSIMP_CFLAGS) \
	-I../contrib/nuklear/


hurtbox_LDADD = \
	$(OPENGL_LIB) \
	$(GLFW_LIBS) \
	$(ASSIMP_LIBS) \
	-ldl \
	-lm
