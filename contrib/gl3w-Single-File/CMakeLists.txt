project(gl3w-Single-File)

set(gl3w-Single-File_headers
    ./gl3w.h
    ./glcorearb.h
)

set (gl3w-Single-File_support 
    ./gl3w_gen.py
)

add_library(gl3w-Single-File INTERFACE
   
)

target_sources(gl3w-Single-File INTERFACE
    ${gl3w-Single-File_headers}
    ${gl3w-Single-File_support}
)