#pragma once

#include "logger.hpp"

namespace LGR {

void Log(const std::string& l_module, const std::string& l_action,
         const std::string& l_event, MessageType type,
         Logger& logger);

}  // namespace LGR