# Create regular library target
add_library(utils STATIC
    src/printer.cpp
)

# Configure include directories
target_include_directories(utils
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    # PRIVATE
    #     ${Boost_INCLUDE_DIRS}
)

# Link Boost libraries
# target_link_libraries(utils
#     PRIVATE
#         Boost::system
#         Boost::filesystem
# )