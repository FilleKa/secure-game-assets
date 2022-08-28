#include <gtest/gtest.h>

#include "writer.hpp"

#include <cstring>
#include <string>

bool CompareArrays(const std::vector<uint8_t> &lhs,
                   const std::vector<uint8_t> &rhs) {
    if (lhs.size() != rhs.size()) {
        return false;
    }

    if (lhs.empty()) {
        return true;
    }

    return std::memcmp(lhs.data(), rhs.data(), lhs.size()) == 0;
}

TEST(Writer, ShouldProperlyWriteString) {
    // Setup
    sga::Writer writer("secret_key");
    std::vector<uint8_t> expected = {'T', 'e', 's', 't', 'i', 'n', 'g'};

    // Execute
    writer.WriteString("Testing");
    writer.Flush();

    // Verify
    ASSERT_TRUE(CompareArrays(writer.GetBuffer(), expected));
}

TEST(Writer, ShouldProperlyWriteInteger) {
    // Setup
    sga::Writer writer("secret_key");
    std::vector<uint8_t> expected = {48, 0, 0, 0, 0, 0, 0, 0};

    // Execute
    writer.Write<uint64_t>(48);
    writer.Flush();

    // Verify
    ASSERT_TRUE(CompareArrays(writer.GetBuffer(), expected));
}

TEST(Writer, ShouldProperlyWriteMultipleData) {
    // Setup
    sga::Writer writer("secret_key");
    std::vector<uint8_t> expected = {48, 0,   0,   0,   0,   0,  0,
                                     0,  'H', 'e', 'l', 'l', 'o'};
    // Execute
    writer.Write<uint64_t>(48);
    writer.WriteString("Hello");
    writer.Flush();

    // Verify
    ASSERT_TRUE(CompareArrays(writer.GetBuffer(), expected));
}

TEST(Writer, ShouldHaveEmptyBufferIfNotFlushed) {
    // Setup
    sga::Writer writer("secret_key");

    // Execute
    writer.WriteString("Testing");

    // Verify
    ASSERT_TRUE(writer.GetBuffer().empty());
}
