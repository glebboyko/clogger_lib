#pragma once

#include <fstream>
#include <list>
#include <mutex>
#include <string>
#include <thread>

namespace LGR {

enum MessageType { Debug = 0, Info = 1, Warning = 2, Error = 3 };

class Logger {
 public:
  Logger(const char* file_name, MessageType mode = Info);
  ~Logger();

  void LogMessage(const std::string& message, MessageType message_type);

 private:
  MessageType mode_;

  std::list<std::string> output_list_;
  std::ofstream file_;
  std::thread output_thread_;
  std::mutex output_lock_;

  bool active_ = true;

  void LogWriter();
};

}  // namespace LGR

#include "source/logger-supply.hpp"