#include <gtest/gtest.h>

#include "endianness.hpp"

TEST(Endianness, ShouldProperlyFlipBytes) {
    // setup
    short data = 1; // 0x00 01 (big endian)

    // execute
    auto endianness = sga::endian::Endianness::kLittle;

    if (sga::endian::GetLocalEndianness() == sga::endian::Endianness::kLittle) {
        endianness = sga::endian::Endianness::kBig;
    }

    sga::endian::ConvertToEndianness(data, endianness);

    // verify
    ASSERT_EQ(data, 0x0100); // converts to 256 (0x01 00 in big endian)
}