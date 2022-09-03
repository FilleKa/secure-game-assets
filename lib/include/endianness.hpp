#ifndef SGA_ENDIANNESS_HPP_
#define SGA_ENDIANNESS_HPP_

#include <cstdint>
#include <stddef.h>
#include <stdio.h>

namespace sga {
namespace endian {

enum class Endianness : uint8_t { kBig, kLittle };

Endianness GetLocalEndianness();

template <typename T> void ConvertToEndianness(T &t, Endianness endianness) {
    if constexpr(sizeof(T) == 1) {
        return;
    }

    if (endianness == GetLocalEndianness()) {
        return;
    }

    auto temp = t;
    auto sz = sizeof(T);
    for (size_t i = 0; i < sz; i++) {
        ((char *)&t)[i] = ((char *)&temp)[sz - (1 + i)];
    }
}

} // namespace endian
} // namespace sga

#endif /* SGA_ENDIANNESS_HPP_ */
