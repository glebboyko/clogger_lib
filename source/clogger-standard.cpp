#include "clogger-standard.hpp"

#include <chrono>

namespace LGR {

/*---------------------------------- supply ----------------------------------*/
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

/*------------------------------ log functions -------------------------------*/
std::string GetRawLog(const std::string& l_module, const std::string& l_action,
                      const std::string& l_event, int type,
                      MessageTypeTranslator type_translator) {
  std::string log = GetCurrTime() + ": " + type_translator(type) + "\n";
  log += "\tfrom " + l_module;
  log += " in " + l_action + ":\n";
  log += "\t\t--- " + l_event + " ---\n";

  return log;
}

std::string GetRawLineLog(const std::string& l_module,
                          const std::string& l_action,
                          const std::string& l_event, int type,
                          MessageTypeTranslator type_translator) {
  std::string log = GetCurrTime() + " " + type_translator(type) + "\t-> " +
                    l_module + "\t-> ";
  if (!l_action.empty()) {
    log += l_action + "\t-> ";
  }

  log += l_event + "\n";

  return log;
}

void Log(const std::string& l_module, const std::string& l_action,
         const std::string& l_event, int type, Logger& logger,
         LogParser log_parser, MessageTypeTranslator type_translator) {
  logger.LogMessage(
      log_parser(l_module, l_action, l_event, type, type_translator), type);
}
/*---------------------------- base logger class -----------------------------*/
StandardBaseLogger::StandardBaseLogger(LGR::Logger& logger,
                                       LGR::StandardBaseLogger::LogFoo log_foo,
                                       LogParser log_parser,
                                       MessageTypeTranslator type_translator)
    : logger_(logger),
      log_foo_(log_foo),
      log_parser_(log_parser),
      type_translator_(type_translator) {}
void StandardBaseLogger::Log(const std::string& event, int priority) {
  log_foo_(GetModule(), GetAction(), event, priority, logger_, log_parser_,
           type_translator_);
}
std::string StandardBaseLogger::GetAction() const { return ""; }

}  // namespace LGR