#pragma once

#include <string>
#include <queue>
#include <mutex>
#include <optional>
#include <condition_variable>

#include "ts_queue.hpp"

namespace ts_queue {

template <typename T>
class TS_BlockingQueue : public TS_Queue<T> {
private:
    std::queue<T> m_queue;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    bool m_closed = false;
public:
    TS_BlockingQueue() = default;
    ~TS_BlockingQueue() = default;

    // Non-copyable
    TS_BlockingQueue(const TS_BlockingQueue&) = delete;
    TS_BlockingQueue& operator=(const TS_BlockingQueue&) = delete;

    auto push(T& value) -> void override {
        std::unique_lock<std::mutex> lock(m_mutex);
        if (m_closed) {
            throw std::runtime_error("Cannot push to closed queue");
        }
        m_queue.emplace(value);
        m_cv.notify_one();
    }

    auto pop(T& value) -> bool override {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_cv.wait(lock, [this] {return !m_queue.empty() || m_closed;});
        if (m_queue.empty()) return false;
        value = std::move(m_queue.front());
        m_queue.pop();
        return true;
    };

    auto close() -> void override {
        std::unique_lock<std::mutex> lock(m_mutex);
        m_closed = true;
        m_cv.notify_all();
    }

    // Status checks
    auto empty() -> bool override {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_queue.empty();
    }
    
    auto size() -> size_t override {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_queue.size();
    }
    
    auto is_closed() -> bool override {
        std::unique_lock<std::mutex> lock(m_mutex);
        return m_closed;
    }
};

} //ts_queue