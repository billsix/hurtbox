project(nuklear_contrib)

set(nuklear_contrib_headers
    nuklear.h
    nuklear_glfw_gl3.h
)

set(nuklear_contrib_source
    main.c
)

add_library(nuklear_contrib ${nuklear_contrib_source} ${nuklear_contrib_headers})