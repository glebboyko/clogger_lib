#include "clogger.hpp"

namespace LGR {

Logger::Logger(const char* file_name, int mode) : mode_(mode) {
  try {
    file_.open(file_name, std::ios::app);
  } catch (std::ofstream::failure& exception) {
    file_.open(file_name, std::ios::out);
  }
  output_thread_ = std::thread(&Logger::LogWriter, this);
}

Logger::~Logger() {
  active_ = false;
  output_semaphore_.try_acquire();
  output_semaphore_.release();

  output_thread_.join();
  file_.close();
}

void Logger::LogMessage(const std::string& message, int message_type) {
  if (message_type > mode_) {
    return;
  }

  buffer_mutex_.lock();
  output_list_.push_back(message);

  output_semaphore_.try_acquire();
  output_semaphore_.release();
  buffer_mutex_.unlock();
}

void Logger::LogWriter() {
  while (true) {
    output_semaphore_.acquire();

    while (!output_list_.empty()) {
      buffer_mutex_.lock();
      std::string message = output_list_.front();
      output_list_.pop_front();
      buffer_mutex_.unlock();
      file_ << message;
    }
    file_.flush();
    if (!active_) {
      return;
    }
  }
}

}  // namespace LGR