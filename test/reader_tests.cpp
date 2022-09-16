#include <gtest/gtest.h>

#include <cstring>
#include <string>

#include "reader.hpp"

TEST(Reader, ShouldProperlyReadString) {
    // Setup
    std::vector<uint8_t> data = {'A', 'B', 'C'};
    sga::Reader reader(std::move(data), "");

    // Execute
    std::string str;
    auto status = reader.ReadString(str, 3);

    // Verify
    ASSERT_EQ(status, sga::Status::kSuccess);
    ASSERT_STREQ(str.c_str(), "ABC");
}

TEST(Reader, ShouldProperlyReadNumber) {
    // Setup
    std::vector<uint8_t> data = {0x01, 0x01, 0x00, 0x00};
    sga::Reader reader(std::move(data), "");

    // Execute
    uint32_t num;
    auto result = reader.Read(num);

    // Verify
    ASSERT_EQ(result, sga::Status::kSuccess);
    ASSERT_EQ(num, 257);
}

TEST(Reader, ShouldProperlyMultipleData) {
    // Setup
    std::vector<uint8_t> data = {'A',  'B',  'C', 0x01, 0x23,
                                 0x45, 0x67, 'X', 'Y'};
    sga::Reader reader(std::move(data), "");

    // Execute
    std::string str_abc;
    auto status_1 = reader.ReadString(str_abc, 3);

    uint32_t num;
    auto status_2 = reader.Read(num);
    std::string str_xy;
    auto status_3 = reader.ReadString(str_xy, 2);

    // Verify

    ASSERT_EQ(status_1, sga::Status::kSuccess);
    ASSERT_EQ(status_2, sga::Status::kSuccess);
    ASSERT_EQ(status_3, sga::Status::kSuccess);

    ASSERT_STREQ(str_abc.c_str(), "ABC");
    ASSERT_EQ(num, 1732584193);
    ASSERT_STREQ(str_xy.c_str(), "XY");
}

TEST(Reader, ShouldReturnBadOperationOnPreparingEmptyEncryptionData) {
    // Setup
    std::vector<uint8_t> data;
    sga::Reader reader(std::move(data), "secret_key");

    // Execute
    auto status = reader.PrepareSize(1, 0);

    // Verify
    ASSERT_EQ(status, sga::Status::kBadOperation);
}

TEST(Reader, ShouldReturnSuccessWhenProperlyPreparingData) {
    // Setup
    std::vector<uint8_t> data;
    for (int i = 0; i < 32; i++) {
        data.push_back(i);
    }

    sga::Reader reader(std::move(data), "secret_key");

    // Execute
    auto status = reader.PrepareSize(4, 0);

    // Verify
    ASSERT_EQ(status, sga::Status::kSuccess);
}

TEST(Reader, ShouldReturnSuccessWhenPreparingNonEncryptedData) {
    // Setup
    std::vector<uint8_t> data;
    sga::Reader reader(std::move(data), "");

    // Execute
    auto status = reader.PrepareSize(1, 0);

    // Verify
    ASSERT_EQ(status, sga::Status::kSuccess);
}