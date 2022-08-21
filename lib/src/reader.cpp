#include "reader.hpp"

namespace sga {
Reader::Reader(const std::string& asset_file_path) {
    input_file_stream_.open(asset_file_path, std::ios::binary | std::ios::in);
}

bool Reader::IsOpen() const {
    return input_file_stream_.is_open();
}

size_t Reader::GetPosition() {
    return input_file_stream_.tellg();
}

void Reader::JumpToPosition(size_t position) {
    input_file_stream_.seekg(position);
}

std::string Reader::ReadString(size_t len) {
    std::string result;
    result.resize(len);

    input_file_stream_.read((char*)result.data(), len);
    return result;
}

} // namespace sga