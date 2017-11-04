project(nuklear_contrib)

set(nuklear_contrib_headers
    ./nuklear.h
    ./nuklear_glfw_gl3.h
)

#assuming this isn't needed
#set(nuklear_contrib_source
    #main.c
#)

add_library(nuklear_contrib INTERFACE)
target_sources(nuklear_contrib INTERFACE
${nuklear_contrib_headers}
)