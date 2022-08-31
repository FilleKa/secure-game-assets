#include "reader.hpp"
#include "aes.hpp"
#include "crypto.hpp"

namespace sga {

Reader::Reader(std::vector<uint8_t> data, const std::string &encryption_key)
    : data_buffer_(data) {

    if (!encryption_key.empty()) {
        padded_key_.resize(encryption_key.length());
        std::memcpy(padded_key_.data(), encryption_key.data(),
                    encryption_key.length());
        Crypto::PadData(padded_key_, 32);
    }

    for (int i = 0; i < 16; i++) {
        nounce_[i] = 0;
    }
}

Reader::Reader(const std::string &asset_file_path,
               const std::string &encryption_key) {
    input_file_stream_.open(asset_file_path, std::ios::binary | std::ios::in);

    if (!encryption_key.empty()) {
        padded_key_.resize(encryption_key.length());
        std::memcpy(padded_key_.data(), encryption_key.data(),
                    encryption_key.length());
        Crypto::PadData(padded_key_, 32);
    }

    for (int i = 0; i < 16; i++) {
        nounce_[i] = 0;
    }
}

Status Reader::PrepareSize(int size) {

    if (!IsUsingEncryption()) {
        return Status::kSuccess;
    }

    auto sz = Crypto::GetPaddedSize(size, 32);
    decrypted_data_.resize(sz);
    decrypted_data_cursor_ = 0;

    if (input_file_stream_.is_open()) {
        input_file_stream_.read((char *)decrypted_data_.data(), sz);
    } else {
        if (static_cast<size_t>(data_buffer_cursor_ + sz) > data_buffer_.size()) {
            return Status::kBadOperation;
        }

        std::memcpy(decrypted_data_.data(), data_buffer_.data() + data_buffer_cursor_, sz);
    }

    AES_ctx ctx;
    AES_init_ctx_iv(&ctx, padded_key_.data(), nounce_.data());
    AES_CBC_decrypt_buffer(&ctx, decrypted_data_.data(), sz);
    
    return Status::kSuccess;
}

bool Reader::IsUsingEncryption() const { return !padded_key_.empty(); }

size_t Reader::GetPosition() { return input_file_stream_.tellg(); }

void Reader::JumpToPosition(size_t position) {
    input_file_stream_.seekg(position);
}

Status Reader::ReadString(std::string &result, size_t len) {
    result.resize(len);

    if (IsUsingEncryption()) {
        std::memcpy((char *)result.data(),
                    decrypted_data_.data() + decrypted_data_cursor_, len);
        decrypted_data_cursor_ += len;
        return Status::kSuccess;
    }

    if (input_file_stream_.is_open()) {
        input_file_stream_.read((char *)result.data(), len);
    } else {
        if (data_buffer_cursor_ + len > data_buffer_.size()) {
            return Status::kBadOperation;
        }

        std::memcpy(result.data(), data_buffer_.data() + data_buffer_cursor_,
                    len);
        data_buffer_cursor_ += len;
    }

    return Status::kSuccess;
}

} // namespace sga