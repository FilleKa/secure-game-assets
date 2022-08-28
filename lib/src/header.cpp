#include "header.hpp"
#include "log.hpp"
#include <iostream>

namespace sga {

constexpr char Header::FORMAT_SIGNATURE[];

Status Header::WriteHeader(Header &header, const std::string &input_folder,
                           sga::Writer &writer) {

    std::string stripped_path = input_folder;
    if (stripped_path.back() == '\\' || stripped_path.back() == '/') {
        stripped_path.pop_back();
    }

    std::filesystem::path input_path(stripped_path);
    std::filesystem::path base = input_path.parent_path();

    log::LogInfo("Packaging resources in: " + base.string());

    std::vector<Header::Entry> entries;

    for (const auto &entry :
         std::filesystem::recursive_directory_iterator(input_path)) {

        if (!entry.is_regular_file()) {
            continue;
        }

        auto path = entry.path().lexically_relative(base);

        log::LogInfo(path.string());

        Header::Entry header_entry;
        header_entry.path = entry.path().string();
        header_entry.packaged_path = path.string();

        entries.push_back(std::move(header_entry));
    }

    writer.WriteString(FORMAT_SIGNATURE);
    writer.FlushEncryped();

    writer.Write<uint64_t>(entries.size());
    writer.FlushEncryped();

    uint64_t header_size = 0;
    uint64_t offset = 0;
    for (auto &e : entries) {
        std::ifstream in_file(e.path, std::ios::ate | std::ios::binary);

        if (!in_file.is_open()) {
            log::LogError("Failed to read file " + e.path);
            return Status::kBadFile;
        }

        e.file_size = in_file.tellg();
        e.offset = offset;

        if (writer.IsUsingEncryption()) {
            offset += Crypto::GetPaddedSize(e.file_size, 32);
        } else {
            offset += e.file_size;
        }

        header_size += 2 * sizeof(uint64_t);
        header_size += e.packaged_path.size();
    }

    writer.Write<uint64_t>(header_size);
    writer.FlushEncryped();

    for (const auto& e : entries) {
        writer.Write<uint64_t>(e.packaged_path.size());
        writer.WriteString(e.packaged_path);
        writer.Write<uint64_t>(e.file_size);
        writer.Write<uint64_t>(e.offset);
    }

    writer.FlushEncryped();

    for (const auto& e : entries) {
        header.entries_.emplace(e.path, e);
    }

    return Status::kSuccess;
}

Status Header::ReadHeader(sga::Reader &reader) {

    reader.PrepareSize(4);
    auto signature = reader.ReadString(4);

    if (signature != FORMAT_SIGNATURE) {
        return Status::kBadFormat;
    }

    reader.PrepareSize(sizeof(uint64_t));
    size_t entry_count = reader.Read<uint64_t>();

    reader.PrepareSize(sizeof(uint64_t));
    size_t header_size = reader.Read<uint64_t>();

    std::cout << "Found " << entry_count << " entries. " << header_size
              << " bytes of data." << std::endl;

    reader.PrepareSize(header_size);

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

bool Header::GetEntry(Entry &entry, const std::string &filename) {
    auto it = entries_.find(filename);

    if (it == entries_.end()) {
        return false;
    }

    entry = it->second;
    return true;
}

const std::map<std::string, Header::Entry>& Header::GetEntries() const {
    return entries_;
}

size_t Header::GetHeaderSize() const { return header_size_; }

} // namespace sga