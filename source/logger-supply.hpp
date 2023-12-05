#pragma once

#include <chrono>

#include "../logger.hpp"

namespace LGR {

#ifdef GLOBAL_LOGGER

struct GlobalLogger {
  static Logger logger;
  static std::string (*log_formatter)(const std::string& l_module,
                                      const std::string& l_action,
                                      const std::string& l_event,
                                      MessageType type);
};

void Log(const std::string& l_module, const std::string& l_action,
         const std::string& l_event, MessageType type) {
  GlobalLogger::logger.LogMessage(
      GlobalLogger::log_formatter(l_module, l_action, l_event, type), type);
}

void Log(const std::string& l_module, const std::string& l_action,
         const std::string& l_event, Logger& logger, MessageType type) {
  logger.LogMessage(
      GlobalLogger::log_formatter(l_module, l_action, l_event, type), type);
}

#ifdef STANDARD_LOGGER

std::string GetCurrTime() {
  using SystemClock = std::chrono::system_clock;
  auto curr_time = SystemClock::to_time_t(SystemClock::now());

  const int kTimeSize = 26;
  std::string buffer;
  buffer.resize(kTimeSize);
  ctime_r(&curr_time, buffer.data());
  buffer.resize(kTimeSize - 2);
  return buffer;
}

std::string MessageTypeToStr(MessageType type) {
  switch (type) {
    case Debug:
      return "DEBUG";
    case Info:
      return "INFO";
    case Warning:
      return "WARNING";
    case Error:
      return "ERROR";
    default:
      return "CANNOT RECOGNIZE TYPE";
  }
}
std::string LogFormatter(const std::string& l_module,
                         const std::string& l_action,
                         const std::string& l_event, MessageType type) {
  std::string log = GetCurrTime() + ": " + MessageTypeToStr(type) + "\n";
  log += "\tfrom " + l_module;
  log += " in " + l_action + ":\n";
  log += "\t\t--- " + l_event + " ---\n\n";

  return log;
}

decltype(GlobalLogger::log_formatter)
    GlobalLogger::log_formatter = LogFormatter;

#endif

#endif

}  // namespace LGR