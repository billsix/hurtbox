project(hurtbox_app)

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

add_executable(hurtbox_app ${hurtbox_headers} ${hurtbox_source})

target_include_directories ( hurtbox_app
    PRIVATE
    ${CMAKE_SOURCE_DIR}/contrib/gl3w-Single-File
    ${CMAKE_SOURCE_DIR}/contrib/stb
    ${CMAKE_SOURCE_DIR}/contrib/nuklear
    ${CMAKE_SOURCE_DIR}/contrib/gl-matrix-c
)

target_link_libraries ( hurtbox_app
    PRIVATE
    "-lm"
    "-ldl"
    ${OPENGL_gl_LIBRARY}
    ${GLFW_LIBRARIES}
    assimp
    gl-matrix-c_contrib
)


install(TARGETS hurtbox_app DESTINATION bin)