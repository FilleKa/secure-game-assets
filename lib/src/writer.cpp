#include "writer.hpp"
#include "crypto.hpp"
#include <cstring>

#include "aes.hpp"

namespace sga {

Writer::Writer(const std::string &encryption_key) : FileBase(encryption_key) {}

Writer::Writer(const std::string &filename, const std::string &encryption_key)
    : FileBase(encryption_key) {
    output_file_stream_.open(filename, std::ios::binary | std::ios::out);
}

Status Writer::FlushEncryped() {
    if (!IsUsingEncryption()) {
        return Status::kBadOperation;
    }

    Crypto::PadData(pending_encrypt_, 32);

    AES_ctx ctx;
    AES_init_ctx_iv(&ctx, GetPaddedKey().data(),
                    GetInitializationVector().data());
    AES_CBC_encrypt_buffer(&ctx, pending_encrypt_.data(),
                           pending_encrypt_.size());

    return Flush();
}

Status Writer::Flush() {
    if (pending_encrypt_.empty()) {
        return Status::kBadOperation;
    }

    if (output_file_stream_.is_open()) {
        output_file_stream_.write((const char *)pending_encrypt_.data(),
                                  pending_encrypt_.size());
    } else {
        std::copy(pending_encrypt_.begin(), pending_encrypt_.end(),
                  std::back_inserter(buffer_));
    }

    pending_encrypt_.clear();
    return Status::kSuccess;
}

std::vector<uint8_t> Writer::GetBuffer() const { return buffer_; }

size_t Writer::GetPosition() { return output_file_stream_.tellp(); }

void Writer::WriteString(const std::string &str) {
    std::copy(str.begin(), str.end(), std::back_inserter(pending_encrypt_));
}

bool Writer::WriteFile(const std::string &path, uint64_t filesize) {
    std::ifstream in_file(path, std::ios::in | std::ios::binary);

    if (!in_file.is_open()) {
        return false;
    }

    auto len = pending_encrypt_.size();
    pending_encrypt_.resize(len + filesize);
    in_file.read((char *)pending_encrypt_.data() + len, filesize);
    return true;
}

} // namespace sga