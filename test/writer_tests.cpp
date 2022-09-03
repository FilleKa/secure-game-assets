#include <gtest/gtest.h>

#include "writer.hpp"

#include <cstring>
#include <string>

bool CompareArrays(const std::vector<uint8_t> &lhs,
                   const std::vector<uint8_t> &rhs,
                   int count = -1) {
    if (count <= 0 && lhs.size() != rhs.size()) {
        return false;
    }

    if (lhs.empty()) {
        return true;
    }

    auto comp_count = count > 0 ? count : lhs.size();

    return std::memcmp(lhs.data(), rhs.data(), comp_count) == 0;
}

TEST(Writer, ShouldReturnSuccessOnFlush) {
    // Setup
    sga::Writer writer("secret_key");

    // Execute
    writer.WriteString("Testing");
    auto result = writer.Flush();

    // Verify
    ASSERT_EQ(result, sga::Status::kSuccess);
}

TEST(Writer, ShouldReturnSuccessOnFlushEncrypted) {
    // Setup
    sga::Writer writer("secret_key");

    // Execute
    writer.WriteString("Testing");
    auto result = writer.FlushEncryped();

    // Verify
    ASSERT_EQ(result, sga::Status::kSuccess);
}

TEST(Writer, ShouldReturnBadOperationOnFlushWithNoData) {
    // Setup
    sga::Writer writer("secret_key");

    // Execute
    auto result = writer.Flush();

    // Verify
    ASSERT_EQ(result, sga::Status::kBadOperation);
}

TEST(Writer, ShouldReturnBadOperationOnFlushEncryptedWithNoKey) {
    // Setup
    sga::Writer writer("");

    // Execute
    auto result = writer.FlushEncryped();

    // Verify
    ASSERT_EQ(result, sga::Status::kBadOperation);
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

TEST(Writer, ShouldUseEncryptionIfKeyProvided) {
    // Setup
    sga::Writer writer("secret_key");

    // Execute
    bool encrypted = writer.IsUsingEncryption();

    // Verify
    ASSERT_TRUE(encrypted);
}

TEST(Writer, ShouldNotUseEncryptionIfKeyNotProvided) {
    // Setup
    sga::Writer writer("");

    // Execute
    bool encrypted = writer.IsUsingEncryption();

    // Verify
    ASSERT_FALSE(encrypted);
}

TEST(Writer, ShouldProperlyEncryptString) {
    // Setup
    sga::Writer writer("secret_key");
    std::vector<uint8_t> expected = {0x0b, 0xb2, 0xbc, 0xd4};

    // Execute
    writer.WriteString("SGAF");
    writer.FlushEncryped();

    // Verify
    ASSERT_TRUE(CompareArrays(writer.GetBuffer(), expected, 4));
}