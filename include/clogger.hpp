#pragma once

#include <fstream>
#include <list>
#include <mutex>
#include <semaphore>
#include <string>
#include <thread>

namespace LGR {

enum MessageType { Debug = 3, Info = 2, Warning = 1, Error = 0 };

class Logger {
 public:
  Logger(const char* file_name, int mode = Info, size_t max_queue = SIZE_MAX);
  Logger(std::ostream& output, int mode = Info, size_t max_queue = SIZE_MAX);

  Logger(const Logger&) = delete;
  Logger(Logger&&) = delete;
  Logger& operator=(const Logger&) = delete;
  Logger& operator=(Logger&&) = delete;

  ~Logger();

  void LogMessage(const std::string& message, int message_type);

 private:
  int mode_;
  size_t max_queue_;

  std::list<std::string> output_list_;
  std::ofstream file_;
  std::thread output_thread_;
  std::binary_semaphore output_semaphore_{0};
  std::mutex buffer_mutex_;

  std::ostream& output_;

  bool active_ = true;

  void LogWriter();
};

}  // namespace LGR