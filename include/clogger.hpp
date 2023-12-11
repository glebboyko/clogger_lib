#pragma once

#include <fstream>
#include <list>
#include <semaphore>
#include <string>
#include <thread>

namespace LGR {

enum MessageType { Debug = 3, Info = 2, Warning = 1, Error = 0 };

class Logger {
 public:
  Logger(const char* file_name, MessageType mode = Info);

  Logger(const Logger&) = delete;
  Logger(Logger&&) = delete;
  Logger& operator=(const Logger&) = delete;
  Logger& operator=(Logger&&) = delete;

  ~Logger();

  void LogMessage(const std::string& message, MessageType message_type);

 private:
  MessageType mode_;

  std::list<std::string> output_list_;
  std::ofstream file_;
  std::thread output_thread_;
  std::binary_semaphore output_semaphore{0};

  bool active_ = true;

  void LogWriter();
};

}  // namespace LGR