#ifndef SGA_CRYPTO_HPP_
#define SGA_CRYPTO_HPP_

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

namespace sga {

class Crypto {
  public:
    static bool IsBigEndian() {
        union {
            uint32_t i;
            char c[4];
        } bint = {0x01020304};

        return bint.c[0] == 1;
    }

    template <typename T> static void MakeLittleEndian(T &t) {
        if (sizeof(T) == 1) {
            return;
        }

        if (!IsBigEndian()) {
            return;
        }

        auto temp = t;
        auto sz = sizeof(T);
        for (size_t i = 0; i < sz; i++) {
            memcpy(&t + i, &temp + (sz - 1 - i), 1);
        }
    }

    static void PadData(std::vector<uint8_t> &data, int bytes) {
        while (data.size() == 0 || (data.size() % bytes) != 0) {
            data.push_back(0);
        }
    }

    static int GetPaddedSize(int size, int bytes) {
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
