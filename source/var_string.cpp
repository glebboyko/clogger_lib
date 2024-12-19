#include "clogger/var_string.hpp"

namespace LGR {

const std::string& VarString::GetData() const noexcept { return data_; }
std::string&& VarString::GetData() noexcept { return std::move(data_); }

void VarString::FromArgs(std::string& output) noexcept {}

}  // namespace LGR