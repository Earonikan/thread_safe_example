# Create header-only library target
add_library(ts_queue INTERFACE)
target_include_directories(ts_queue
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)