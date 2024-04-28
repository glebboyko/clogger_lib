#pragma once

#include <functional>

#include "clogger.hpp"

namespace LGR {

// supply
using MessageTypeTranslator = std::function<std::string(int)>;
using LogParser =
    std::function<std::string(const std::string&, const std::string&,
                              const std::string&, int, MessageTypeTranslator)>;

std::string MessageTypeToStr(int type);

// log functions
std::string GetRawLog(const std::string& l_module, const std::string& l_action,
                      const std::string& l_event, int type,
                      MessageTypeTranslator type_translator = MessageTypeToStr);
std::string GetRawLineLog(
    const std::string& l_module, const std::string& l_action,
    const std::string& l_event, int type,
    MessageTypeTranslator type_translator = MessageTypeToStr);

void Log(const std::string& l_module, const std::string& l_action,
         const std::string& l_event, int type, Logger& logger,
         LogParser log_parser = GetRawLog,
         MessageTypeTranslator type_translator = MessageTypeToStr);

// base logger class
class StandardBaseLogger {
 public:
  void Log(const std::string& event, int priority);

 protected:
  using LogFoo = std::function<void(const std::string&, const std::string&,
                                    const std::string&, int, Logger&, LogParser,
                                    MessageTypeTranslator)>;

  StandardBaseLogger(Logger& logger, LogFoo log_foo = ::LGR::Log,
                     LogParser log_parser = GetRawLog,
                     MessageTypeTranslator type_translator = MessageTypeToStr);

  virtual std::string GetModule() const = 0;
  virtual std::string GetAction() const;

 private:
  Logger& logger_;
  LogFoo log_foo_;
  LogParser log_parser_;
  MessageTypeTranslator type_translator_;
};

}  // namespace LGR