#ifndef SGA_FILE_HPP_
#define SGA_FILE_HPP_

#include <cstdint>
#include <memory>

namespace sga {

class SGAFile {
  public:
    SGAFile(std::unique_ptr<uint8_t[]>&& data, size_t size);

    size_t GetSize() const;

    template<typename T>
    T* GetPointer() {
        if (!data_) {
            return nullptr;
        }

        return reinterpret_cast<T*>(data_.get());
    }

  private:
    std::unique_ptr<uint8_t[]> data_;
    size_t size_;
};

} // namespace sga

#endif /* SGA_FILE_HPP_ */
