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
    ${gl-matrix-c_contrib_headers}
    ${stb_contrib_headers}
    ${nuklear_contrib_headers}
    ${gl3w-Single-File_contrib_headers}
)

target_link_libraries ( hurtbox_app
    PRIVATE
    gl-matrix-c_contrib
    nuklear_contrib
)
