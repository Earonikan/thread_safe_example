#pragma once

#include <string>

namespace ts_queue {

class Printer {
    std::string m_msg;
public:
    auto SetMessage(std::string msg) -> void;
    auto Print() -> void;
};

} //ts_queue