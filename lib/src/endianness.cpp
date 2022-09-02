#include "endianness.hpp"

namespace sga {
namespace endian {

Endianness GetLocalEndianness() {
    union {
        uint32_t i;
        char c[4];
    } bint = {0x01020304};

    return (bint.c[0] == 1) ? Endianness::kBig : Endianness::kLittle;
}

} // namespace endian
} // namespace sga