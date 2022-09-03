#ifndef SGA_CRYPTO_HPP_
#define SGA_CRYPTO_HPP_

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

namespace sga {

class Crypto {
  public:

    static void PadData(std::vector<uint8_t> &data, int bytes) {
        while (data.size() == 0 || (data.size() % bytes) != 0) {
            data.push_back(0);
        }
    }

    static size_t GetPaddedSize(size_t size, size_t bytes) {
        if (size == 0) {
            return bytes;
        }

        if (size % bytes == 0) {
            return size;
        }

        return (1 + (size / bytes)) * bytes;
    }
};

} // namespace sga

#endif /* SGA_CRYPTO_HPP_ */
