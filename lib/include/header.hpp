#ifndef SGA_HEADER_HPP_
#define SGA_HEADER_HPP_

#include "reader.hpp"
#include "status.hpp"
#include "writer.hpp"

#include <filesystem>
#include <map>
#include <string>

namespace sga {

class Header {
  public:
    Header() = default;

    static Status WriteHeader(Header& header, const std::string &input_folder, sga::Writer& writer);
    Status ReadHeader(sga::Reader &reader);

    static constexpr char FORMAT_SIGNATURE[] = "SGAF";

    struct Entry {
        std::string path;
        std::string packaged_path;
        uint64_t file_size;
        uint64_t offset;
    };

    bool GetEntry(Entry &entry, const std::string &filename);

    const std::map<std::string, Entry>& GetEntries() const;

    size_t GetHeaderSize() const;

  private:
    size_t header_size_;
    std::map<std::string, Entry> entries_;
};
} // namespace sga

#endif /* SGA_HEADER_HPP_ */
