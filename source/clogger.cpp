#include "clogger.hpp"

namespace LGR {

Logger::Logger(const char* file_name, MessageType mode) : mode_(mode) {
  try {
    file_.open(file_name, std::ios::app);
  } catch (std::ofstream::failure& exception) {
    file_.open(file_name, std::ios::out);
  }
  output_thread_ = std::thread(&Logger::LogWriter, this);
}

Logger::~Logger() {
  active_ = false;
  output_semaphore.try_acquire();
  output_semaphore.release();

  output_thread_.join();
  file_.close();
}

void Logger::LogMessage(const std::string& message, MessageType message_type) {
  if (message_type > mode_) {
    return;
  }

  output_list_.push_back(message);

  output_semaphore.try_acquire();
  output_semaphore.release();
}

void Logger::LogWriter() {
  while (true) {
    output_semaphore.acquire();

    while (!output_list_.empty()) {
      file_ << output_list_.front();
      output_list_.pop_front();
    }
    if (!active_) {
      return;
    }
  }
}

}  // namespace LGR