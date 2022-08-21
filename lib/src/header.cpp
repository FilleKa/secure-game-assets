#include "header.hpp"
#include <iostream>

namespace sga {

constexpr char Header::FORMAT_SIGNATURE[];

Status Header::ReadHeader(sga::Reader &reader) {

    auto signature = reader.ReadString(4);

    if (signature != FORMAT_SIGNATURE) {
        return Status::kBadFormat;
    }

    size_t entry_count = reader.Read<uint64_t>();
    std::cout << "Found " << entry_count << " entries" << std::endl;

    for (int i = 0; i < entry_count; i++) {

        size_t filename_len = reader.Read<uint64_t>();
        std::cout << "len:" << filename_len << std::endl;

        auto filepath = reader.ReadString(filename_len);
        std::cout << filepath << std::endl;

        size_t filesize = reader.Read<uint64_t>();
        std::cout << filesize << std::endl;
        size_t offset = reader.Read<uint64_t>();
        std::cout << offset << std::endl;

        Entry entry;
        entry.path = filepath;
        entry.packaged_path = filepath;
        entry.offset = offset;
        entry.file_size = filesize;
        entries_.emplace(filepath, std::move(entry));
    }

    header_size_ = reader.GetPosition();
    return Status::kSuccess;
}

bool Header::GetEntry(Entry& entry, const std::string& filename) {
    auto it = entries_.find(filename);

    if (it == entries_.end()) {
        return false;
    }

    entry = it->second;
    return true;
}

size_t Header::GetHeaderSize() const {
    return header_size_;
}


} // namespace sga