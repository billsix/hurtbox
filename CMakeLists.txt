cmake_minimum_required(VERSION 3.0)
project(hurtbox)

include(CheckIncludeFiles)

#find dependencies

if(APPLE OR WIN32)
else()
    set(OpenGL_GL_PREFERENCE "GLVND")
    find_package(PkgConfig REQUIRED)
    pkg_search_module(GLFW REQUIRED glfw3)
    find_package(OpenGL REQUIRED)
endif()


add_definitions(-DPACKAGE_NAME="hurtbox")
add_definitions(-DPACKAGE_TARNAME="hurtbox")
add_definitions(-DPACKAGE_VERSION="0.0.1")
add_definitions(-DPACKAGE_STRING="hurtbox 0.0.1")
add_definitions(-DPACKAGE_BUGREPORT="William Emerison Six <billsix@gmail.com>")
add_definitions(-DPACKAGE_URL="")


#not quite sure why ${CMAKE_INSTALL_PREFIX} resolves correctly, but ${DESTINATION} doesn't
add_definitions(-DTEXTURE_DIR="${CMAKE_INSTALL_PREFIX}/share/hurtbox/textures/")
add_definitions(-DSHADER_DIR="${CMAKE_INSTALL_PREFIX}/share/hurtbox/shaders/")
add_definitions(-DMODELS_DIR="${CMAKE_INSTALL_PREFIX}/share/hurtbox/models/")

add_definitions(-DCMAKE_CONFIG)

#CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/config.h.in ${CMAKE_CURRENT_BINARY_DIR}/config.h)

add_subdirectory(shaders)
add_subdirectory(src)

install(DIRECTORY models/ DESTINATION share/hurtbox/models)
install(DIRECTORY shaders/ DESTINATION share/hurtbox/shaders)
install(DIRECTORY textures/ DESTINATION share/hurtbox/textures)
