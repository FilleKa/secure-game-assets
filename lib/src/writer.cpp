#include "writer.hpp"
#include "crypto.hpp"
#include <cstring>

#include "aes.hpp"

namespace sga {

Writer::Writer(const std::string &filename, const std::string &encryption_key) {
    output_file_stream_.open(filename, std::ios::binary | std::ios::out);

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

bool Writer::IsOpen() const { return output_file_stream_.is_open(); }

bool Writer::IsUsingEncryption() const { return !padded_key_.empty(); }

void Writer::FlushEncryped() {
    if (!IsUsingEncryption()) {
        Flush();
        return;
    }

    Crypto::PadData(pending_encrypt_, 32);

    AES_ctx ctx;
    AES_init_ctx_iv(&ctx, padded_key_.data(), nounce_.data());
    AES_CBC_encrypt_buffer(&ctx, pending_encrypt_.data(),
                           pending_encrypt_.size());
    output_file_stream_.write((char *)pending_encrypt_.data(),
                              pending_encrypt_.size());
    pending_encrypt_.clear();
}

void Writer::Flush() {
    output_file_stream_.write((const char *)pending_encrypt_.data(),
                              pending_encrypt_.size());
    pending_encrypt_.clear();
}

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
    in_file.read((char*) pending_encrypt_.data() + len, filesize);
    return true;
}

} // namespace sga