#include "log.hpp"

namespace sga {
namespace log {

void LogInfo(const std::string& info) {
    std::cout << "[info]: " << info.c_str() << std::endl;
}

void LogError(const std::string& error) {
    std::cout << "[error]: " << error.c_str() << std::endl;
}

} // namespace log
} // namespace sga