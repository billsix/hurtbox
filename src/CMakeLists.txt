project(hurtbox)

set(hurtbox_headers
    ./common.h
    ./controller.h
    ./intro-scene.h
    ./load_asset.h
    ./main.h
    ./mainscene.h
    ./shader.h
)

set(hurtbox_source
    ./controller.c
    ./intro-scene.c
    ./load_asset.c
    ./main.c
    ./mainscene.c
    ./shader.c
)

# TODO - make the fetching and building of these static libraries
# possible on linux through an option
if(APPLE OR WIN32)
     set(BUILD_SHARED_LIBS OFF)
     if(WIN32)
         # no console on start
         set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:WINDOWS /ENTRY:mainCRTStartup")
     endif(WIN32)
endif()

if(WIN32)
    add_subdirectory(${CMAKE_SOURCE_DIR}/contrib/glfw)
    include_directories(${CMAKE_SOURCE_DIR}/contrib/glfw/include)
endif(WIN32)



add_executable(hurtbox ${hurtbox_headers} ${hurtbox_source})

if(WIN32)
    target_link_libraries(hurtbox glfw opengl32 )
endif(WIN32)

target_include_directories ( hurtbox
    PRIVATE
    ${CMAKE_SOURCE_DIR}/contrib/gl3w-Single-File
    ${CMAKE_SOURCE_DIR}/contrib/stb
    ${CMAKE_SOURCE_DIR}/contrib/nuklear
    ${CMAKE_SOURCE_DIR}/contrib/gl-matrix-stack/
)

target_link_libraries ( hurtbox
    PRIVATE
    "-lm"
    "-ldl"
    ${OPENGL_gl_LIBRARY}
    ${GLFW_LIBRARIES}
    assimp
)


install(TARGETS hurtbox DESTINATION bin)
