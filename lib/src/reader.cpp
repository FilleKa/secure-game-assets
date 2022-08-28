#include "reader.hpp"
#include "aes.hpp"
#include "crypto.hpp"

namespace sga {
Reader::Reader(const std::string &asset_file_path,
               const std::string &encryption_key) {
    input_file_stream_.open(asset_file_path, std::ios::binary | std::ios::in);

    if (encryption_key != "") {
        padded_key_.resize(encryption_key.length());
        std::memcpy(padded_key_.data(), encryption_key.data(),
                    encryption_key.length());
        Crypto::PadData(padded_key_, 32);
    }

    for (int i = 0; i < 16; i++) {
        nounce_[i] = 0;
    }
}

void Reader::PrepareSize(int size) {

    if (!IsUsingEncryption()) {
        return;
    }

    auto sz = Crypto::GetPaddedSize(size, 32);
    decrypted_data_.resize(sz);
    decrypted_data_cursor_ = 0;
    input_file_stream_.read((char *)decrypted_data_.data(), sz);

    AES_ctx ctx;
    AES_init_ctx_iv(&ctx, padded_key_.data(), nounce_.data());
    AES_CBC_decrypt_buffer(&ctx, decrypted_data_.data(), sz);
}

bool Reader::IsUsingEncryption() const { return !padded_key_.empty(); }

bool Reader::IsOpen() const { return input_file_stream_.is_open(); }

size_t Reader::GetPosition() { return input_file_stream_.tellg(); }

void Reader::JumpToPosition(size_t position) {
    input_file_stream_.seekg(position);
}

std::string Reader::ReadString(size_t len) {
    std::string result;
    result.resize(len);

    if (IsUsingEncryption()) {
        std::memcpy((char*) result.data(), decrypted_data_.data() + decrypted_data_cursor_, len);
        decrypted_data_cursor_ += len;
        return result;
    }

    input_file_stream_.read((char *)result.data(), len);
    return result;
}

} // namespace sga