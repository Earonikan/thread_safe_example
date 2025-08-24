#include "worker.hpp"

namespace ts_queue {

    auto Worker::SetTask(Task task, uint64_t num) -> void {
        m_task = task;
        m_thread = std::thread(&Task::InvokeN, m_task, num);
    }
    auto Worker::GetTask() -> Task {
        return m_task;
    }
    auto Worker::DoTask() -> void {
        // m_task->Invoke();
        m_thread.join();
    }
    // auto Worker::DoTaskN(uint64_t num) -> void {
    //     m_task->InvokeN(num);
    //     m_thread.join();
    // }

} //ts_queue