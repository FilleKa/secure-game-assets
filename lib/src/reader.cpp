#include "reader.hpp"
#include "aes.hpp"
#include "crypto.hpp"

namespace sga {

Reader::Reader(std::vector<uint8_t>&& data, const std::string& encryption_key)
    : FileBase(encryption_key), data_buffer_(std::move(data)) {}

Reader::Reader(const std::string& asset_file_path,
               const std::string& encryption_key)
    : FileBase(encryption_key) {
    input_file_stream_.open(asset_file_path, std::ios::binary | std::ios::in);
}

bool Reader::IsFileOpen() const { return input_file_stream_.is_open(); }

Status Reader::PrepareSize(size_t size, uint64_t message_index) {

    if (!IsUsingEncryption()) {
        return Status::kSuccess;
    }

    auto sz = Crypto::GetPaddedSize(size, 32);
    decrypted_data_.resize(sz);
    decrypted_data_cursor_ = 0;

    if (input_file_stream_.is_open()) {
        input_file_stream_.read((char*)decrypted_data_.data(), sz);
    } else {
        if (data_buffer_cursor_ + sz > data_buffer_.size()) {
            return Status::kBadOperation;
        }

        std::memcpy(decrypted_data_.data(),
                    data_buffer_.data() + data_buffer_cursor_, sz);
        data_buffer_cursor_ += sz;
    }

    AES_ctx ctx;
    AES_init_ctx_iv(&ctx, GetPaddedKey().data(),
                    GetInitializationVector(message_index).data());
    AES_CBC_decrypt_buffer(&ctx, decrypted_data_.data(), sz);

    return Status::kSuccess;
}

size_t Reader::GetPosition() { return input_file_stream_.tellg(); }

void Reader::JumpToPosition(size_t position) {
    input_file_stream_.seekg(position);
}

Status Reader::ReadString(std::string& result, size_t len) {
    result.resize(len);

    if (IsUsingEncryption()) {
        std::memcpy((char*)result.data(),
                    decrypted_data_.data() + decrypted_data_cursor_, len);
        decrypted_data_cursor_ += len;
        return Status::kSuccess;
    }

    if (input_file_stream_.is_open()) {
        input_file_stream_.read((char*)result.data(), len);
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

Status Reader::ReadData(std::unique_ptr<uint8_t[]>& data, size_t len) {
    data = std::make_unique<uint8_t[]>(len + 1);

    // makes it easier to print data for debug purposes..
    data.get()[len] = '\0';

    if (IsUsingEncryption()) {
        std::memcpy((char*)data.get(),
                    decrypted_data_.data() + decrypted_data_cursor_, len);
        decrypted_data_cursor_ += len;
        return Status::kSuccess;
    }

    if (input_file_stream_.is_open()) {
        input_file_stream_.read((char*)data.get(), len);
    } else {
        if (data_buffer_cursor_ + len > data_buffer_.size()) {
            return Status::kBadOperation;
        }

        std::memcpy(data.get(), data_buffer_.data() + data_buffer_cursor_, len);
        data_buffer_cursor_ += len;
    }

    return Status::kSuccess;
}

} // namespace sga