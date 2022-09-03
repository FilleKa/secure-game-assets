#include "file_base.hpp"
#include "crypto.hpp"
#include "endianness.hpp"

#include <cstring>

namespace sga {
FileBase::FileBase(const std::string& encryption_key) {
    if (!encryption_key.empty()) {
        padded_key_.resize(encryption_key.length());
        std::memcpy(padded_key_.data(), encryption_key.data(),
                    encryption_key.length());
        Crypto::PadData(padded_key_, 32);
    }
}

bool FileBase::IsUsingEncryption() const { return !padded_key_.empty(); }

const std::vector<uint8_t>& FileBase::GetPaddedKey() const {
    return padded_key_;
}

std::array<uint8_t, 16>
FileBase::GetInitializationVector(uint64_t message_index) const {

    std::array<uint8_t, 16> iv;

    std::memcpy(iv.data(), InitializationVector, 8);

    endian::ConvertToEndianness(message_index, endian::Endianness::kLittle);

    memcpy(iv.data() + 8, &message_index, 8);

    return iv;
}

} // namespace sga