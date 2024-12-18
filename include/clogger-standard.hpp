#pragma once

#include <map>
#include <memory>

#include "clogger.hpp"

namespace LGR {

class StandardLogger : protected LGR::Logger {
 public:
  StandardLogger(const std::string& name, int mode = MessageType::Info,
                 bool async = true, char delimiter = '-');
  StandardLogger(const std::string& name, const char* file_name,
                 int mode = MessageType::Info, bool async = true,
                 char delimiter = '-', size_t max_queue = SIZE_MAX);
  StandardLogger(const std::string& name, std::ostream& output,
                 int mode = MessageType::Info, bool async = true,
                 char delimiter = '-', size_t max_queue = SIZE_MAX);

  StandardLogger(const StandardLogger&) = default;
  StandardLogger(StandardLogger&&) = default;
  StandardLogger& operator=(const StandardLogger&) = default;
  StandardLogger& operator=(StandardLogger&&) = default;

  ~StandardLogger() = default;

  virtual bool Debug(const std::string& msg) noexcept;
  virtual bool Info(const std::string& msg) noexcept;
  virtual bool Warning(const std::string& msg) noexcept;
  virtual bool Error(const std::string& msg) noexcept;
  virtual bool Critical(const std::string& msg) noexcept;

  virtual bool Log(const std::string& msg, MessageType priority) noexcept;

  virtual StandardLogger GetChild(const std::string& suffix,
                                  bool add_index = false) const;
  virtual StandardLogger GetChild(const std::string& suffix, MessageType mode,
                                  bool add_index = false) const;

  virtual void ChangeMode(int logging_level);

 protected:
  std::string name_;
  char delimiter_;

  mutable std::map<std::string, size_t> child_indexes_;
};

}  // namespace LGR