#ifndef SGA_READER_HPP_
#define SGA_READER_HPP_

#include <array>
#include <cstring>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include "file_base.hpp"
#include "status.hpp"

namespace sga {

class Reader : public FileBase {
  public:
    Reader(const std::string& asset_file_path,
           const std::string& encryption_key);

    Reader(std::vector<uint8_t>&& data, const std::string& encryption_key);

    bool IsFileOpen() const;

    Status PrepareSize(size_t size, uint64_t message_index);

    template<typename T>
    Status Read(T& result) {
        auto len = sizeof(T);

        if (IsUsingEncryption()) {
            auto src = decrypted_data_.data() + decrypted_data_cursor_;
            std::memcpy((char*)&result, src, len);
            decrypted_data_cursor_ += len;
            return Status::kSuccess;
        }

        if (input_file_stream_.is_open()) {
            input_file_stream_.read((char*)&result, sizeof(T));
        } else {

            if (data_buffer_cursor_ + sizeof(T) > data_buffer_.size()) {
                return Status::kBadOperation;
            }

            std::memcpy(&result, data_buffer_.data() + data_buffer_cursor_,
                        sizeof(T));
            data_buffer_cursor_ += sizeof(T);
        }

        return Status::kSuccess;
    }

    size_t GetPosition();
    void JumpToPosition(size_t position);

    Status ReadString(std::string& result, size_t len);
    Status ReadData(std::unique_ptr<uint8_t[]>& data, size_t len);

  private:
    std::ifstream input_file_stream_;

    std::vector<uint8_t> decrypted_data_;
    size_t decrypted_data_cursor_ = 0;

    std::vector<uint8_t> data_buffer_;
    size_t data_buffer_cursor_ = 0;
};

} // namespace sga

#endif /* SGA_READER_HPP_ */
