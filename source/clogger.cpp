#include "clogger.hpp"

#include "clogger-standard.hpp"

namespace LGR {

Logger::Writer::Writer(const char* file_name, bool async, size_t max_queue)
    : async_(async), max_queue_(max_queue), output_(file_) {
  try {
    file_.open(file_name, std::ios::app);
  } catch (std::ofstream::failure& exception) {
    file_.open(file_name, std::ios::out);
  }

  if (async_) {
    worker_thread_ = std::thread(&Writer::Worker, this);
  }
}

Logger::Writer::Writer(std::ostream& output, bool async, size_t max_queue)
    : async_(async), max_queue_(max_queue), output_(output) {
  if (async_) {
    worker_thread_ = std::thread(&Writer::Worker, this);
  }
}

Logger::Writer::~Writer() {
  if (async_) {
    active_ = false;
    output_semaphore_.try_acquire();
    output_semaphore_.release();

    worker_thread_.join();
  }
}

void Logger::Writer::Write(const std::string& message) {
  if (async_) {
    buffer_mutex_.lock();
    output_list_.push_back(message);

    output_semaphore_.try_acquire();
    output_semaphore_.release();
    buffer_mutex_.unlock();
  } else {
    output_ << message << "\n";
  }
}

void Logger::Writer::Worker() noexcept {
  try {
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
  } catch (std::exception& exception) {
    try {
      output_ << "caught exception while execution worker: " << exception.what()
              << "\n";
      output_.flush();
    } catch (...) {
    }
  } catch (...) {
    try {
      output_ << "caught unknown while execution worker\n";
      output_.flush();
    } catch (...) {
    }
  }
}

Logger::Logger(const char* file_name, int mode, bool async, size_t max_queue)
    : mode_(mode),
      writer_(std::make_shared<Writer>(file_name, async, max_queue)) {}

Logger::Logger(std::ostream& output, int mode, bool async, size_t max_queue)
    : mode_(mode),
      writer_(std::make_shared<Writer>(output, async, max_queue)) {}

bool Logger::Log(const std::string& message, int message_type) noexcept {
  try {
    if (message_type < mode_) {
      return false;
    }

    writer_->Write(message);
    return true;
  } catch (...) {
    return false;
  }
}

}  // namespace LGR