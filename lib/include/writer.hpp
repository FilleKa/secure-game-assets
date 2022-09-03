#ifndef SGA_WRITER_HPP_
#define SGA_WRITER_HPP_

#include <array>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>


#include "crypto.hpp"
#include "endianness.hpp"
#include "file_base.hpp"
#include "status.hpp"


namespace sga {

class Writer : public FileBase {
  public:
    Writer(const std::string& encryption_key);
    Writer(const std::string& filename, const std::string& encryption_key);

    Status FlushEncryped();
    Status Flush();

    std::vector<uint8_t> GetBuffer() const;

    template <typename T> void Write(const T& input) {

        auto data = input;
        endian::ConvertToEndianness(data, endian::Endianness::kLittle);

        auto len = pending_encrypt_.size();
        pending_encrypt_.resize(len + sizeof(T));
        std::memcpy(pending_encrypt_.data() + len, &data, sizeof(T));
    }

    size_t GetPosition();

    void WriteString(const std::string& str);

    bool WriteFile(const std::filesystem::path& path, uint64_t filesize);

  private:
    std::ofstream output_file_stream_;
    std::vector<uint8_t> pending_encrypt_;
    std::vector<uint8_t> buffer_;
    uint64_t flushed_message_count_ = 0;
};

} // namespace sga

#endif /* SGA_WRITER_HPP_ */
