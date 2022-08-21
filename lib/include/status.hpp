#ifndef SGA_STATUS_HPP_
#define SGA_STATUS_HPP_

#include <cstdint>

namespace sga {
    enum class Status : uint8_t {
        kSuccess,
        kBadFormat
    };
}

#endif /* SGA_STATUS_HPP_ */
