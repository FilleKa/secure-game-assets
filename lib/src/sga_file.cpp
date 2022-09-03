#include "sga_file.hpp"

namespace sga {

SGAFile::SGAFile(std::unique_ptr<uint8_t[]>&& data, size_t size) : data_(std::move(data)), size_(size) {}

size_t SGAFile::GetSize() const { return size_; }

} // namespace sga