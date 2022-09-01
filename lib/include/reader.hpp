#ifndef SGA_READER_HPP_
#define SGA_READER_HPP_

#include <array>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>

#include "status.hpp"
#include "file_base.hpp"

namespace sga {

class Reader : public FileBase {
  public:
    Reader(const std::string &asset_file_path,
           const std::string &encryption_key);

    Reader(std::vector<uint8_t> data, const std::string &encryption_key);

    Status PrepareSize(int size);

    template <typename T> Status Read(T &result) {
        auto len = sizeof(T);

        if (IsUsingEncryption()) {
            auto src = decrypted_data_.data() + decrypted_data_cursor_;
            std::memcpy((char *)&result, src, len);
            decrypted_data_cursor_ += len;
            return Status::kSuccess;
        }

        if (input_file_stream_.is_open()) {
            input_file_stream_.read((char *)&result, sizeof(T));
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

    Status ReadString(std::string &result, size_t len);

  private:
    std::ifstream input_file_stream_;

    std::vector<uint8_t> decrypted_data_;
    int decrypted_data_cursor_ = 0;

    std::vector<uint8_t> data_buffer_;
    int data_buffer_cursor_ = 0;
};

} // namespace sga

#endif /* SGA_READER_HPP_ */
