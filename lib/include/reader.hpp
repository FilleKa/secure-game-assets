#ifndef SGA_READER_HPP_
#define SGA_READER_HPP_

#include <fstream>
#include <string>

namespace sga {

class Reader {
public:
    Reader(const std::string& asset_file_path);

    bool IsOpen() const;

    template<typename T>
    T Read() {
        T result;
        input_file_stream_.read((char*) &result, sizeof(T));
        return result;
    }

    size_t GetPosition();
    void JumpToPosition(size_t position);

    std::string ReadString(size_t len);

private:
    std::ifstream input_file_stream_;
};

} // namespace sga

#endif /* SGA_READER_HPP_ */
