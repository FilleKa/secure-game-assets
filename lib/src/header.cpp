#include "header.hpp"
#include "log.hpp"

namespace sga {

constexpr char Header::FORMAT_SIGNATURE[];

Status Header::WriteHeader(Header& header, const std::string& input_folder,
                           sga::Writer& writer) {

    size_t flushed_bytes = 0;

    std::string stripped_path = input_folder;
    if (stripped_path.back() == '\\' || stripped_path.back() == '/') {
        stripped_path.pop_back();
    }

    std::filesystem::path input_path(stripped_path);
    std::filesystem::path base = input_path.parent_path();

    log::LogInfo("Packaging resources in: " + base.string());

    std::vector<Header::Entry> entries;

    for (const auto& entry :
         std::filesystem::recursive_directory_iterator(input_path)) {

        if (!entry.is_regular_file()) {
            continue;
        }

        auto path = entry.path().lexically_relative(base);

        log::LogInfo(path.string());

        Header::Entry header_entry;
        header_entry.path = entry.path();
        header_entry.packaged_path = path;

        entries.push_back(std::move(header_entry));
    }

    writer.WriteString(FORMAT_SIGNATURE);
    writer.FlushEncryped(flushed_bytes);

    writer.Write<uint64_t>(entries.size());
    writer.FlushEncryped(flushed_bytes);

    uint64_t header_size = 0;
    uint64_t offset = 0;
    for (auto& e : entries) {
        std::ifstream in_file(e.path, std::ios::ate | std::ios::binary);

        if (!in_file.is_open()) {
            log::LogError("Failed to read file " + e.path.string());
            return Status::kBadFile;
        }

        e.file_size = in_file.tellg();
        e.offset = offset;

        if (writer.IsUsingEncryption()) {
            offset += Crypto::GetPaddedSize(e.file_size, 32);
        } else {
            offset += e.file_size;
        }
        
        auto nice_path = e.packaged_path.make_preferred().string();
        std::replace(nice_path.begin(), nice_path.end(), '\\', '/');

        header_size += nice_path.size();
        header_size += 3 * sizeof(uint64_t);
    }

    writer.Write<uint64_t>(header_size);
    writer.FlushEncryped(flushed_bytes);

    for (auto& e : entries) {

        auto nice_path = e.packaged_path.make_preferred().string();
        std::replace(nice_path.begin(), nice_path.end(), '\\', '/');

        writer.Write<uint64_t>(nice_path.size());
        writer.WriteString(nice_path);
        writer.Write<uint64_t>(e.file_size);
        writer.Write<uint64_t>(e.offset);
    }

    writer.FlushEncryped(flushed_bytes);

    for (const auto& e : entries) {
        header.entries_.emplace(e.path.string(), e);
    }

    return Status::kSuccess;
}

Status Header::ReadHeader(sga::Reader& reader) {

    uint64_t message_count = 0;
    auto status = reader.PrepareSize(4, message_count++);
    
    if (status != Status::kSuccess) {
        return status;
    }

    std::string signature;
    status = reader.ReadString(signature, 4);

    if (status != Status::kSuccess) {
        return status;
    }

    if (signature != FORMAT_SIGNATURE) {
        return Status::kBadFormat;
    }

    reader.PrepareSize(sizeof(uint64_t), message_count++);
    uint64_t entry_count;
    status = reader.Read(entry_count);

    if (status != Status::kSuccess) {
        return status;
    }

    reader.PrepareSize(sizeof(uint64_t), message_count++);
    uint64_t header_size;
    status = reader.Read(header_size);

    if (status != Status::kSuccess) {
        return status;
    }

    reader.PrepareSize(header_size, message_count++);

    for (uint64_t i = 0; i < entry_count; i++) {

        uint64_t filename_len;
        status = reader.Read(filename_len);
        
        if (status != Status::kSuccess) {
            return status;
        }

        std::string filepath;
        status = reader.ReadString(filepath, filename_len);

        if (status != Status::kSuccess) {
            return status;
        }

        uint64_t filesize;
        status = reader.Read(filesize);

        if (status != Status::kSuccess) {
            return status;
        }

        uint64_t offset;
        status = reader.Read(offset);

        if (status != Status::kSuccess) {
            return status;
        }

        Entry entry;
        entry.path = filepath;
        entry.path.make_preferred();
        entry.packaged_path = entry.path;
        entry.offset = offset;
        entry.file_size = filesize;
        entry.index = 10 + i;

        entries_.emplace(filepath, std::move(entry));
    }

    header_size_ = reader.GetPosition();
    return Status::kSuccess;
}

bool Header::GetEntry(Entry& entry, const std::string& filename) const {
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

const std::string& Header::GetAssetFilename() const { return asset_filename_; }

void Header::SetAssetFilename(const std::string& filename) {
    asset_filename_ = filename;
}

const std::string& Header::GetEncryptionKey() const { return encryption_key_; }

void Header::SetEncryptionKey(const std::string& encryption_key) {
    encryption_key_ = encryption_key;
}

} // namespace sga