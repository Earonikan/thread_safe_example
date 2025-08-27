#include <iostream>
#include "printer.hpp"

namespace ts_queue {
    auto Printer::GetCurrentTime() -> std::string {
        auto now = std::time(nullptr);
        std::tm timeinfo;
        localtime_r(&now, &timeinfo);
        
        std::stringstream ss;
        ss << std::put_time(&timeinfo, "%Y-%m-%d %H:%M:%S");
        return ss.str();
    }
    auto Printer::SetMessage(std::string msg) -> void {
        m_msg = msg;
    }
    auto Printer::Print() -> void {
        std::cout << "[" << GetCurrentTime() << "] " << m_msg << std::endl;
    }

} //ts_queue