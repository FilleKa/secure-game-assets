#include "log.hpp"

namespace sga {
namespace log {

void LogInfo(const std::string& info) {
    std::cout << "[info]: " << info << std::endl;
}

void LogError(const std::string& error) {
    std::cout << "[error]: " << error << std::endl;
}

} // namespace log
} // namespace sga