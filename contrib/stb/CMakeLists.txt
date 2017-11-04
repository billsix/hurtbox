project(stb_contrib)

set(stb_contrib_headers
    ./stb_image.h
)

add_library(stb_contrib INTERFACE )

target_sources(stb_contrib INTERFACE
    ${stb_contrib_headers}
)