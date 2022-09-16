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
    static Status WriteHeader(Header& header, const std::string& input_folder,
                              sga::Writer& writer);
    Status ReadHeader(sga::Reader& reader);

    static constexpr char FORMAT_SIGNATURE[] = "SGAF";

    struct Entry {
        std::filesystem::path path;
        std::filesystem::path packaged_path;
        uint64_t file_size;
        uint64_t offset;
        uint64_t index;
    };

    bool GetEntry(Entry& entry, const std::string& filename) const;

    const std::map<std::string, Entry>& GetEntries() const;

    size_t GetHeaderSize() const;

    const std::string& GetAssetFilename() const;
    void SetAssetFilename(const std::string& filename);

    const std::string& GetEncryptionKey() const;
    void SetEncryptionKey(const std::string& encryption_key);

  private:
    size_t header_size_;
    std::map<std::string, Entry> entries_;
    std::string asset_filename_;
    std::string encryption_key_;
};
} // namespace sga

#endif /* SGA_HEADER_HPP_ */
