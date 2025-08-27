#pragma once

#include <string>
#include <ctime>
#include <algorithm>
#include <iomanip>

namespace ts_queue {

class Printer {
    std::string m_msg;
    auto GetCurrentTime() -> std::string;
public:
    auto SetMessage(std::string msg) -> void;
    auto Print() -> void;
};

} //ts_queue