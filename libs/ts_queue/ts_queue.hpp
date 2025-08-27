#pragma once

namespace ts_queue {

template <typename T>
class TS_Queue {
public:
    TS_Queue() = default;
    ~TS_Queue() = default;

    // Non-copyable
    TS_Queue(const TS_Queue&) = delete;
    TS_Queue& operator=(const TS_Queue&) = delete;

    virtual auto push(T& value) -> void = 0;

    virtual auto pop(T& value) -> bool = 0;

    virtual auto close() -> void = 0;

    virtual auto empty() -> bool = 0;
    
    virtual auto size() -> size_t = 0;
    
    virtual auto is_closed() -> bool = 0;
};

} //ts_queue