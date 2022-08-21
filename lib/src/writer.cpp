#include "writer.hpp"

namespace sga {

    Writer::Writer(const std::string& filename) {
        output_file_stream_.open(filename, std::ios::binary | std::ios::out);
    }

    bool Writer::IsOpen() const {
        return output_file_stream_.is_open();
    }

    size_t Writer::GetPosition() {
        return output_file_stream_.tellp();
    }

    void Writer::WriteString(const std::string& str) {
        output_file_stream_.write((char*)str.c_str(), str.length());
    }

    bool Writer::WriteFile(const std::string& path) {
        std::ifstream in_file(path, std::ios::in | std::ios::binary);

        if (!in_file.is_open()) {
            return false;
        }

        std::copy(std::istreambuf_iterator<char>(in_file),
                std::istreambuf_iterator<char>(),
                std::ostreambuf_iterator<char>(output_file_stream_));
        return true;
    }

} // namespace sga