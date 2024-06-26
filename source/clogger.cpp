#include "clogger.hpp"

#include "clogger-standard.hpp"

namespace LGR {

Logger::Logger(const char* file_name, int mode, bool async, size_t max_queue)
    : mode_(mode), async_(async), max_queue_(max_queue), output_(file_) {
  try {
    file_.open(file_name, std::ios::app);
  } catch (std::ofstream::failure& exception) {
    file_.open(file_name, std::ios::out);
  }

  if (async_) {
    output_thread_ = std::thread(&Logger::LogWriter, this);
  }
}
Logger::Logger(std::ostream& output, int mode, bool async, size_t max_queue)
    : mode_(mode), async_(async), max_queue_(max_queue), output_(output) {
  if (async_) {
    output_thread_ = std::thread(&Logger::LogWriter, this);
  }
}

Logger::~Logger() {
  if (async_) {
    active_ = false;
    output_semaphore_.try_acquire();
    output_semaphore_.release();

    output_thread_.join();
  }
}

void Logger::LogMessage(const std::string& message, int message_type) {
  if (message_type > mode_) {
    return;
  }

  if (async_) {
    buffer_mutex_.lock();
    output_list_.push_back(message);

    output_semaphore_.try_acquire();
    output_semaphore_.release();
    buffer_mutex_.unlock();
  } else {
    output_ << message << "\n";
    output_.flush();
  }
}

void Logger::LogWriter() {
  while (true) {
    output_semaphore_.acquire();

    bool contiguous = false;

    while (!output_list_.empty()) {
      if (!contiguous) {
        buffer_mutex_.lock();
        if (output_list_.size() > max_queue_) {
          output_list_.push_front(GetRawLineLog(
              "LOGGER", "LOG WRITER", "Max logger queue reached", Warning));
          contiguous = true;
        }
      }

      std::string message = output_list_.front();
      output_list_.pop_front();
      if (!contiguous) {
        buffer_mutex_.unlock();
      }
      output_ << message << "\n";
    }
    output_.flush();

    if (contiguous) {
      buffer_mutex_.unlock();
      contiguous = false;
    }

    if (!active_) {
      return;
    }
  }
}

}  // namespace LGR