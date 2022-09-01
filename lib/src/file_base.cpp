#include "file_base.hpp"
#include "crypto.hpp"
#include <cstring>

namespace sga {
FileBase::FileBase(const std::string &encryption_key) {
    if (!encryption_key.empty()) {
        padded_key_.resize(encryption_key.length());
        std::memcpy(padded_key_.data(), encryption_key.data(),
                    encryption_key.length());
        Crypto::PadData(padded_key_, 32);
    }

    // todo(fk): fix IV
    for (int i = 0; i < 16; i++) {
        iv_[i] = 0;
    }
}

bool FileBase::IsUsingEncryption() const { return !padded_key_.empty(); }

const std::vector<uint8_t> &FileBase::GetPaddedKey() const {
    return padded_key_;
}

const std::array<uint8_t, 16> &FileBase::GetInitializationVector() const {
    return iv_;
}

} // namespace sga