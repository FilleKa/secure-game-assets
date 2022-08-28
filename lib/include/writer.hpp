#ifndef SGA_WRITER_HPP_
#define SGA_WRITER_HPP_

#include <array>
#include <cstring>
#include <fstream>
#include <string>
#include <vector>

#include "crypto.hpp"

namespace sga {

class Writer {
  public:
    Writer(const std::string &filename, const std::string &encryption_key);
    bool IsOpen() const;
    bool IsUsingEncryption() const;

    void FlushEncryped();
    void Flush();

    template <typename T> void Write(const T &input) {
        auto len = pending_encrypt_.size();
        pending_encrypt_.resize(len + sizeof(T));
        std::memcpy(pending_encrypt_.data() + len, &input, sizeof(T));
    }

    size_t GetPosition();

    void WriteString(const std::string &str);

    bool WriteFile(const std::string &path, uint64_t filesize);

    void InitializeNounce();

  private:
    std::ofstream output_file_stream_;
    std::vector<uint8_t> pending_encrypt_;
    std::vector<uint8_t> padded_key_;
    std::array<uint8_t, 16> nounce_;
};

} // namespace sga

#endif /* SGA_WRITER_HPP_ */
