#pragma once

#include <functional>
#include <memory>
#include <string>

namespace ts_queue {

class Task {
private:
    static uint64_t m_uuid_cnt;
    std::string m_name;
    uint64_t m_uuid;
    std::function<void()> m_task;
    
public:
    Task();
    Task(std::string name);
    Task(const Task& other) = default;
    Task(Task&& other) = default;

    auto operator=(const Task& other) -> Task& = default;
    auto operator=(Task&& other) -> Task& = default;

    template <class T, typename... Args>
    auto SetCallback(void (T::*fun1)(Args...), std::shared_ptr<T> t, Args... params) -> void {
        m_task = [t, fun1, params...] () { return (t.get()->*fun1)(params...); };
    }
    template <class T, typename... Args>
    auto SetCallback(void (T::*fun1)(Args...), T *t, Args... params) -> void {
        m_task = [t, fun1, params...] () { return (t->*fun1)(params...); };
    }
    template <class T, typename... Args>
    auto SetCallback(void (T::*fun1)(Args...), std::shared_ptr<T> t) -> void {
        m_task = [t, fun1] () { return (t.get()->*fun1)(); };
    }
    template <class T, typename... Args>
    auto SetCallback(void (T::*fun1)(Args...), T *t) -> void {
        m_task = [t, fun1] () { return (t->*fun1)(); };
    }
    template <class T>
    auto SetCallback(std::function<void()> fun1) -> void {
        m_task = fun1;
    }
    auto Invoke() -> void;
    auto InvokeN(uint64_t num) -> void;
    static auto IncEventId() -> void;
    auto GetUuid() -> uint64_t;
};

} //ts_queue