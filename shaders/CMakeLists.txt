project(shaders)

add_library(shaders INTERFACE)
target_sources(shaders INTERFACE    
    ./walls.frag
    ./walls.vert)