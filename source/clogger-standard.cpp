#include <chrono>

#include "clogger-standard.hpp"

namespace LGR {

std::string GetCurrTime() {
  using SystemClock = std::chrono::system_clock;
  auto now = SystemClock::now();
  auto curr_time = SystemClock::to_time_t(now);

  const int kTimeSize = 26;
  std::string buffer;
  buffer.resize(kTimeSize);
  ctime_r(&curr_time, buffer.data());
  buffer.resize(kTimeSize - 2);

  int precision = (now - SystemClock::from_time_t(curr_time)).count();
  buffer += " " + std::to_string(precision);
  return buffer;
}

std::string MessageTypeToStr(int type) {
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
                         const std::string& l_event, int type) {
  std::string log = GetCurrTime() + ": " + MessageTypeToStr(type) + "\n";
  log += "\tfrom " + l_module;
  log += " in " + l_action + ":\n";
  log += "\t\t--- " + l_event + " ---\n\n";

  return log;
}

void Log(const std::string& l_module, const std::string& l_action,
         const std::string& l_event, int type, Logger& logger) {
  logger.LogMessage(LogFormatter(l_module, l_action, l_event, type), type);
}

}  // namespace LGR