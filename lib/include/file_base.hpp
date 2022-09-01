#ifndef SGA_FILE_BASE_HPP_
#define SGA_FILE_BASE_HPP_

#include <array>
#include <cstdint>
#include <string>
#include <vector>

namespace sga {

class FileBase {
  public:
    FileBase(const std::string &encryption_key);

    bool IsUsingEncryption() const;

  protected:
    const std::vector<uint8_t> &GetPaddedKey() const;
    std::array<uint8_t, 16>
    GetInitializationVector(uint64_t message_index) const;

  private:
    static constexpr uint8_t InitializationVector[16] = {
        0xca, 0xfe, 0xba, 0xbe, 0xca, 0xfe, 0xba,  0xbe,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x000, 0x00};
    std::vector<uint8_t> padded_key_;
};

} // namespace  sga

#endif /* SGA_FILE_BASE_HPP_ */
