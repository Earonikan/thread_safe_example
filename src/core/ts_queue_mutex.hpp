#pragma once

#include <string>
#include <queue>
#include <mutex>
#include <optional>

namespace ts_queue {

template <typename T>
class ts_queue_mutex {
private:
    std::queue<T> m_queue;
    std::mutex m_mutex;
public:
    auto push(T& value) -> void {
        const std::scoped_lock<std::mutex> lock(m_mutex);
        m_queue.emplace(value);
    }
    auto empty() -> bool {
        const std::scoped_lock<std::mutex> lock(m_mutex);
        bool value = m_queue.empty();
        return value;
    };
    auto pop(T& value) -> void {
        const std::scoped_lock<std::mutex> lock(m_mutex);
        if (!m_queue.empty()) {
            value = m_queue.front();
            m_queue.pop();
            // return value;
        }
        // return std::nullopt;
    };
    // auto back() -> T {
    //     const std::scoped_lock<std::mutex> lock(m_mutex);
    //     T value = m_queue.back();
    //     return value;
    // };
    // auto pop() -> void {
    //     const std::scoped_lock<std::mutex> lock(m_mutex);
    //     if (!m_queue.empty())
    //         m_queue.pop();
    // };
    // auto push(T& value) -> void {
    //     const std::scoped_lock<std::mutex> lock(m_mutex);
    //     m_queue.push(value);
    // };
    // auto size() -> size_t {
    //     const std::scoped_lock<std::mutex> lock(m_mutex);
    //     size_t value = m_queue.size();
    //     return value;
    // };
    // auto swap(ts_queue_mutex& other) -> void {
    //     const std::scoped_lock<std::mutex> lock(m_mutex);
    //     m_queue.swap(other);
    // };
    // auto &operator=(const ts_queue_mutex& other) {
    //     const std::scoped_lock<std::mutex> lock(m_mutex);
    //     m_queue = other.m_queue;
    // }
    // auto &operator=(const ts_queue_mutex&& other) {
    //     const std::scoped_lock<std::mutex> lock(m_mutex);
    //     m_queue = std::move(other.m_queue);
    // }
};

} //ts_queue