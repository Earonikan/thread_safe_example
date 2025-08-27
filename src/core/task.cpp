#include <iostream>
#include "core/task.hpp"

namespace ts_queue {
    Task::Task() : m_uuid(m_uuid_cnt++) {}
    Task::Task(std::string name) : m_name(name), m_uuid(m_uuid_cnt++) {}
    auto Task::Invoke() -> void {
        if (m_task) {
            std::cout << "Performing " << m_name << "\t" << m_uuid_cnt << std::endl;
            m_task();
        }
    }
    auto Task::InvokeN(uint64_t num) -> void {
        if (m_task) {
            for (uint64_t i{0}; i < num; ++i) {
                std::cout << "Performing #" << i << "\t" << m_name << std::endl;
                m_task();
            }
        }
    }

    auto Task::IncEventId() -> void {m_uuid_cnt++;}

    auto Task::GetUuid() -> uint64_t {return m_uuid_cnt;}

    uint64_t Task::m_uuid_cnt = 0;

} //ts_queue