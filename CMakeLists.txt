cmake_minimum_required(VERSION 3.20)

project(StepMania)

if(LINUX)
    add_compile_options ( -fPIC )
endif()

include(StepmaniaCore.cmake)

# The external libraries need to be included.
add_subdirectory(extern)

# The internal libraries and eventual executable to be used.
add_subdirectory(src)

# Optional plugins people might want to compile
add_subdirectory(Plugins)
