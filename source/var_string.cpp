#include "clogger/var_string.hpp"

namespace LGR {

const std::string& VarString::GetData() const noexcept { return data_; }

void VarString::FromArgs(std::string& output) noexcept {}

}  // namespace LGR