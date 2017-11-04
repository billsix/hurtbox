project(gl3w-Single-File_contrib)

set(gl3w-Single-File_contrib_headers
    ./gl3w.h
    ./glcorearb.h
)

set (gl3w-Single-File_contrib_support 
    ./gl3w_gen.py
)

add_library(gl3w-Single-File_contrib INTERFACE
   
)

target_sources(gl3w-Single-File_contrib INTERFACE
    ${gl3w-Single-File_contrib_headers}
    ${gl3w-Single-File_contrib_support}
)