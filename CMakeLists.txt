cmake_minimum_required(VERSION 3.20)

project(thread_safe_queue)

# find_program(CLANG_TIDY_EXE NAMES "clang-tidy" REQUIRED)

# Set include dirs
include_directories(src)

# setup clang-tidy command from executable + options
# set(CLANG_TIDY_COMMAND "${CLANG_TIDY_EXE}" "-config-file=${CMAKE_SOURCE_DIR}/.clang-tidy")

set(Boost_USE_STATIC_LIBS OFF) 
set(Boost_USE_MULTITHREADED ON)  
set(Boost_USE_STATIC_RUNTIME OFF) 
find_package(Boost 1.74.0 COMPONENTS lockfree) 

if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
endif()

add_subdirectory(src)

add_executable(thread_safe_queue main.cpp)

# Linker
target_link_libraries(thread_safe_queue PRIVATE core ${Boost_LIBRARIES})

# Set C++ standard
target_compile_features(thread_safe_queue PRIVATE cxx_std_20)

# Set clang-tidy checks
set_target_properties(thread_safe_queue PROPERTIES CXX_CLANG_TIDY "${CLANG_TIDY_COMMAND}")

# Set compiler flags
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang" )
  target_compile_options(thread_safe_queue PRIVATE 
    -Wall -Wextra -Wundef -Wshadow -Wcast-align -Wpointer-arith -Wcast-qual 
    -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code 
  )
endif()
