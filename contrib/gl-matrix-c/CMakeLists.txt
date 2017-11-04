project(gl-matrix-c_contrib)

set(gl-matrix-c_contrib_headers
    ./gl-matrix.h
)

set(gl-matrix-c_contrib_source
    ./mat3.c
    ./mat4.c
    ./quat.c
    ./str.c
    ./vec3.c
)

add_library(gl-matrix-c_contrib ${gl-matrix-c_contrib_source} ${gl-matrix-c_contrib_headers})