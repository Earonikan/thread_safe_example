add_library(common
    common.cpp
)

# Specify include directories for the library
target_include_directories(common
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
)

target_compile_options(common PRIVATE 
    -Wall -Wextra -Wundef -Wshadow -Wcast-align 
    -Wpointer-arith -Wcast-qual 
    -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code 
)

target_compile_features(common PRIVATE cxx_std_20)