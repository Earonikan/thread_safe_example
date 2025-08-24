#include <iostream>
#include "printer.hpp"

namespace ts_queue {

    auto Printer::SetMessage(std::string msg) -> void {
        m_msg = msg;
    }
    auto Printer::Print() -> void {
        std::cout << m_msg << std::endl;
    }

} //ts_queue