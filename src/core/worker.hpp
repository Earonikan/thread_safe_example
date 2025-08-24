#pragma once

#include <string>
#include <thread>
#include "task.hpp"

namespace ts_queue {

class Worker {
    std::thread m_thread;
    Task m_task;
public:
    auto SetTask(Task task, uint64_t num) -> void;
    auto GetTask() -> Task;
    auto DoTask() -> void;
    // auto DoTaskN(uint64_t num) -> void;
};

} //ts_queue