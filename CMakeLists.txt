cmake_minimum_required(VERSION 3.0)
project(hurtbox)

include(CheckIncludeFiles)

#find dependencies

find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
pkg_search_module(GLFW REQUIRED assimp)
find_package(OpenGL REQUIRED)

CHECK_INCLUDE_FILES("dlfcn.h" HAVE_DLFCN_H)
CHECK_INCLUDE_FILES("inttypes.h" HAVE_INTTYPES_H)
CHECK_INCLUDE_FILES("memory.h" HAVE_MEMORY_H)
CHECK_INCLUDE_FILES("stdint.h" HAVE_STDINT_H)
CHECK_INCLUDE_FILES("stdlib.h" HAVE_STDLIB_H)
CHECK_INCLUDE_FILES("strings.h" HAVE_STRINGS_H)
CHECK_INCLUDE_FILES("string.h" HAVE_STRING_H)
CHECK_INCLUDE_FILES("sys/stat.h" HAVE_SYS_STAT_H)
CHECK_INCLUDE_FILES("sys/types.h" HAVE_SYS_TYPES_H)
CHECK_INCLUDE_FILES("unistd.h" HAVE_UNISTD_H)
CHECK_INCLUDE_FILES("stdlib.h stddef.h" STDC_HEADERS)

add_definitions(-DPACKAGE_NAME="hurtbox")
add_definitions(-DPACKAGE_TARNAME="hurtbox")
add_definitions(-DPACKAGE_VERSION="0.0.1")
add_definitions(-DPACKAGE_STRING="hurtbox 0.0.1")
add_definitions(-DPACKAGE_BUGREPORT="William Emerison Six <billsix@gmail.com>")
add_definitions(-DPACKAGE_URL="")

#I'm assuming these are rel to 'bin' in the install...
add_definitions(-DTEXTURE_DIR="../share/hurtbox/textures/")
add_definitions(-DSHADER_DIR="../share/hurtbox/shaders/")
add_definitions(-DMODELS_DIR="../share/hurtbox/models/")

add_definitions(-DCMAKE_CONFIG)

#CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

add_subdirectory(shaders)
add_subdirectory(contrib)
add_subdirectory(src)

install(DIRECTORY src/ DESTINATION share/hurtbox
    FILES_MATCHING PATTERN "*.h")
install(DIRECTORY models/ DESTINATION share/hurtbox/models)
install(DIRECTORY shaders/ DESTINATION share/hurtbox/shaders)
install(DIRECTORY textures/ DESTINATION share/hurtbox/textures)