#ifndef SGA_ENDIANNESS_HPP_
#define SGA_ENDIANNESS_HPP_

#include <cstdint>
#include <stdio.h>
#include <stddef.h>

namespace sga {
namespace endian {

enum class Endianness : uint8_t { kBig, kLittle };

Endianness GetLocalEndianness();

template <typename T> void ConvertToEndianness(T &t, Endianness endianness) {
    if (sizeof(T) == 1) {
        return;
    }

    if (endianness == GetLocalEndianness()) {
        return;
    }

    auto temp = t;
    auto sz = sizeof(T);
    for (size_t i = 0; i < sz; i++) {
        memcpy(&t + i, &temp + (sz - 1 - i), 1);
    }
}

} // namespace endian
} // namespace sga

#endif /* SGA_ENDIANNESS_HPP_ */
