#include <gtest/gtest.h>

#include <cstring>
#include <string>

#include "header.hpp"
#include "reader.hpp"

TEST(Header, ShouldReturnBadOperationOnEmptyData) {
    // Setup
    sga::Header header;
    sga::Reader reader(std::vector<uint8_t>(), "");

    // Execute
    auto result = header.ReadHeader(reader);

    // Verify
    ASSERT_EQ(result, sga::Status::kBadOperation);
}

TEST(Header, ShouldReturnBadFormatIfFormatNotRecognized) {
    // Setup
    sga::Header header;
    sga::Reader reader(std::vector<uint8_t>{0xde, 0xad, 0xbe, 0xef}, "");

    // Execute
    auto result = header.ReadHeader(reader);

    // Verify
    ASSERT_EQ(result, sga::Status::kBadFormat);
}