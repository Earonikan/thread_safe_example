add_library(core 
    printer.hpp
    printer.cpp
    ts_queue_mutex.hpp
    task.hpp
    task.cpp
    worker.hpp
    worker.cpp
)

target_compile_options(core PRIVATE 
    -Wall -Wextra -Wundef -Wshadow -Wcast-align 
    -Wpointer-arith -Wcast-qual 
    -Wswitch-default -Wswitch-enum -Wconversion -Wunreachable-code 
)
target_compile_features(core PRIVATE cxx_std_20)
