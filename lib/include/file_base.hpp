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
    const std::array<uint8_t, 16> &GetInitializationVector() const;

  private:
    std::vector<uint8_t> padded_key_;
    std::array<uint8_t, 16> iv_;
};

} // namespace  sga

#endif /* SGA_FILE_BASE_HPP_ */
