#pragma once

#include "clogger.hpp"

namespace LGR {

void Log(const std::string& l_module, const std::string& l_action,
         const std::string& l_event, int type,
         Logger& logger);

}  // namespace LGR