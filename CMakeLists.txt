cmake_minimum_required(VERSION 3.15)
project(ThreadSafeApp VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find dependencies (for regular_lib)
# find_package(Boost 1.75 REQUIRED COMPONENTS system filesystem)

# Configure header-only library
add_subdirectory(libs/ts_queue)

# Configure regular library
add_subdirectory(libs/utils)

# Configure src
add_subdirectory(src)

# Create main executable
add_executable(app
    main.cpp
)

# Linker
target_link_libraries(app PRIVATE core common)

# Set include directories for main app
target_include_directories(app
    PRIVATE
        include
        libs/utils/include
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# Link dependencies
target_link_libraries(app
    PRIVATE
        ts_queue
        utils
        # Boost::system
        # Boost::filesystem
)

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)