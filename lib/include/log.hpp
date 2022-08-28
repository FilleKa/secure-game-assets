#ifndef SGA_LOG_HPP_
#define SGA_LOG_HPP_

#include <iostream>

namespace sga {
namespace log {

void LogInfo(const std::string& info);
void LogError(const std::string& error);

} // namespace log
} // namespace sga

#endif /* SGA_LOG_HPP_ */