#pragma once

#include <sstream>
#include <string>

namespace LGR {

class VarString {
 public:
  template <typename... Args>
  VarString(const Args&... args) noexcept {
    FromArgs(data_, args...);
  }

  const std::string& GetData() const noexcept;
  std::string&& GetData() noexcept;

 private:
  std::string data_;

  template <typename Head, typename... Args>
  void FromArgs(std::string& output, const Head& head,
                const Args&... tail) noexcept {
    try {
      std::stringstream stream;
      stream << head;

      auto str = stream.str();
      if (!output.empty()) {
        output.push_back(' ');
      }
      output += str;
    } catch (...) {
    }

    FromArgs(output, tail...);
  }

  void FromArgs(std::string& output) noexcept;
};

}  // namespace LGR