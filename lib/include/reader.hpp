#ifndef SGA_READER_HPP_
#define SGA_READER_HPP_

#include <array>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>

namespace sga {

class Reader {
  public:
    Reader(const std::string &asset_file_path,
           const std::string &encryption_key);

    void PrepareSize(int size);

    bool IsOpen() const;
    bool IsUsingEncryption() const;

    template <typename T> T Read() {

        T result;
        auto len = sizeof(T);

        if (IsUsingEncryption()) {
            auto src = decrypted_data_.data() + decrypted_data_cursor_;
            std::memcpy((char *)&result, src, len);
            decrypted_data_cursor_ += len;
            return result;
        }

        input_file_stream_.read((char *)&result, sizeof(T));
        return result;
    }

    size_t GetPosition();
    void JumpToPosition(size_t position);

    std::string ReadString(size_t len);

  private:
    std::ifstream input_file_stream_;
    std::vector<uint8_t> decrypted_data_;
    std::vector<uint8_t> padded_key_;
    std::array<uint8_t, 16> nounce_;

    int decrypted_data_cursor_ = 0;
};

} // namespace sga

#endif /* SGA_READER_HPP_ */
