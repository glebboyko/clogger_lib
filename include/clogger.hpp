#pragma once

#include <fstream>
#include <list>
#include <memory>
#include <mutex>
#include <semaphore>
#include <string>
#include <thread>

namespace LGR {

enum MessageType {
  Debug = 10,
  Info = 20,
  Warning = 30,
  Error = 40,
  Critical = 50
};

class Logger {
 public:
  Logger(const char* file_name, int mode = Info, bool async = true,
         size_t max_queue = SIZE_MAX);
  Logger(std::ostream& output, int mode = Info, bool async = true,
         size_t max_queue = SIZE_MAX);

  Logger(const Logger&) = default;
  Logger(Logger&&) = default;
  Logger& operator=(const Logger&) = default;
  Logger& operator=(Logger&&) = default;

  ~Logger() = default;

  virtual bool Log(const std::string& message, int message_type) noexcept;

 private:
  class Writer {
   public:
    Writer(const char* file_name, bool async, size_t max_queue);
    Writer(std::ostream& output, bool async, size_t max_queue);

    Writer(const Writer&) = delete;
    Writer(Writer&&) = delete;
    Writer& operator=(const Writer&) = delete;
    Writer& operator=(Writer&&) = delete;

    ~Writer();

    void Write(const std::string& message);

   private:
    std::list<std::string> output_list_;
    bool async_;
    size_t max_queue_;

    std::ofstream file_;
    std::ostream& output_;

    std::thread worker_thread_;
    std::binary_semaphore output_semaphore_{0};
    std::mutex buffer_mutex_;

    bool active_ = true;

    void Worker() noexcept;
  };

 protected:
  int mode_;

 private:
  std::shared_ptr<Writer> writer_;
};

}  // namespace LGR