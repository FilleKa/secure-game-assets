#ifndef SGA_HEADER_HPP_
#define SGA_HEADER_HPP_

#include "status.hpp"

#include <map>
#include <reader.hpp>
#include <string>

namespace sga {

class Header {
  public:

    Status ReadHeader(sga::Reader& reader);

    static constexpr char FORMAT_SIGNATURE[] = "SGAF";

    struct Entry {
        std::string path;
        std::string packaged_path;
        size_t file_size;
        size_t offset;
    };

    bool GetEntry(Entry &entry, const std::string &filename);

    size_t GetHeaderSize() const;

  private:
    size_t header_size_;
    std::map<std::string, Entry> entries_;
};
} // namespace sga

#endif /* SGA_HEADER_HPP_ */
