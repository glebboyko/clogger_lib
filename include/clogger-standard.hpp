#pragma once

#include "clogger.hpp"

namespace LGR {

std::string GetRawLog(const std::string& l_module, const std::string& l_action,
                      const std::string& l_event, int type);

void Log(const std::string& l_module, const std::string& l_action,
         const std::string& l_event, int type, Logger& logger);

}  // namespace LGR