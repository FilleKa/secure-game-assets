#ifndef SGA_WRITER_HPP_
#define SGA_WRITER_HPP_

#include <fstream>
#include <string>

namespace sga {

class Writer {
    public:
    Writer(const std::string& filename);
    bool IsOpen() const;

    template<typename T>
    void Write(const T& input) {
        output_file_stream_.write((const char*)(&input), sizeof(T));
    }

    size_t GetPosition();
    
    void WriteString(const std::string& str);

    bool WriteFile(const std::string& path);

private:
    std::ofstream output_file_stream_;
};

} // namespace sga

#endif /* SGA_WRITER_HPP_ */
