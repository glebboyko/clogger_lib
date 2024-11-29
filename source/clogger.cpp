#include "clogger.hpp"

#include "clogger-standard.hpp"

namespace LGR {

Logger::Writer::Writer(const char* file_name, size_t max_queue)
    : max_queue_(max_queue), output_(file_) {
  try {
    file_.open(file_name, std::ios::app);
  } catch (std::ofstream::failure& exception) {
    file_.open(file_name, std::ios::out);
  }
  worker_thread_ = std::thread(&Writer::Worker, this);
}

Logger::Writer::Writer(std::ostream& output, size_t max_queue)
    : max_queue_(max_queue), output_(output) {
  worker_thread_ = std::thread(&Writer::Worker, this);
}

Logger::Writer::~Writer() {
  active_ = false;
  output_semaphore_.try_acquire();
  output_semaphore_.release();

  worker_thread_.join();
}

void Logger::Writer::Write(const std::string& message) {
  buffer_mutex_.lock();
  output_list_.push_back(message);

  output_semaphore_.try_acquire();
  output_semaphore_.release();
  buffer_mutex_.unlock();
}

void Logger::Writer::Worker() {
  while (true) {
    output_semaphore_.acquire();

    bool contiguous = false;

    while (!output_list_.empty()) {
      if (!contiguous) {
        buffer_mutex_.lock();
        if (output_list_.size() > max_queue_) {
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

Logger::Logger(const char* file_name, int mode, size_t max_queue)
    : mode_(mode), writer_(std::make_shared<Writer>(file_name, max_queue)) {}

Logger::Logger(std::ostream& output, int mode, size_t max_queue)
    : mode_(mode), writer_(std::make_shared<Writer>(output, max_queue)) {}

void Logger::Log(const std::string& message, int message_type) {
  if (message_type < mode_) {
    return;
  }

  writer_->Write(message);
}

}  // namespace LGR