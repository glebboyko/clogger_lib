#include "clogger-standard.hpp"

#include <chrono>
#include <iostream>

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
      return "   DEBUG";
    case Info:
      return "    INFO";
    case Warning:
      return " WARNING";
    case Error:
      return "   ERROR";
    case Critical:
      return "CRITICAL";
    default:
      return " UNKNOWN";
  }
}

StandardLogger::StandardLogger(const std::string& name, LGR::MessageType mode,
                               bool async, char delimiter)
    : StandardLogger(name, std::cerr, mode, async, delimiter) {}

StandardLogger::StandardLogger(const std::string& name, std::ostream& output,
                               LGR::MessageType mode, bool async,
                               char delimiter, size_t max_queue)
    : LGR::Logger(output, mode, async, max_queue),
      name_(name),
      delimiter_(delimiter) {}

StandardLogger::StandardLogger(const std::string& name, const char* file_name,
                               LGR::MessageType mode, bool async,
                               char delimiter, size_t max_queue)
    : LGR::Logger(file_name, mode, async, max_queue),
      name_(name),
      delimiter_(delimiter) {}

StandardLogger StandardLogger::GetChild(const std::string& suffix,
                                        bool add_index) const {
  return GetChild(suffix, static_cast<MessageType>(mode_), add_index);
}

StandardLogger StandardLogger::GetChild(const std::string& suffix,
                                        LGR::MessageType mode,
                                        bool add_index) const {
  StandardLogger child = *this;
  child.name_ = name_ + delimiter_ + suffix;
  child.mode_ = mode;
  child.child_indexes_.clear();

  if (add_index) {
    auto iter = child_indexes_.find(child.name_);
    if (iter != child_indexes_.end()) {
      iter->second += 1;
      child.name_ += std::to_string(iter->second);
    } else {
      child_indexes_.insert({child.name_, 1});
      child.name_ += '1';
    }
  }

  return child;
}

void StandardLogger::Debug(const std::string& msg) {
  Log(msg, MessageType::Debug);
}
void StandardLogger::Info(const std::string& msg) {
  Log(msg, MessageType::Info);
}
void StandardLogger::Warning(const std::string& msg) {
  Log(msg, MessageType::Warning);
}
void StandardLogger::Error(const std::string& msg) {
  Log(msg, MessageType::Error);
}
void StandardLogger::Critical(const std::string& msg) {
  Log(msg, MessageType::Critical);
}

void StandardLogger::Log(const std::string& msg, LGR::MessageType priority) {
  Logger::Log(GetCurrTime() + '\t' + MessageTypeToStr(priority) + '\t' + name_ +
                  " :\t" + msg,
              priority);
}

}  // namespace LGR