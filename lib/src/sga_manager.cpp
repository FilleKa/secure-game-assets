#include "sga_manager.hpp"
#include "reader.hpp"

#include "aes.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace sga {

Status SGAManager::OpenAssetFile(const std::string& asset_file_path,
                                 const std::string& encryption_key) {
    Reader reader(asset_file_path, encryption_key);

    if (!reader.IsFileOpen()) {
        return Status::kBadFile;
    }

    auto header = std::make_unique<Header>();
    auto status = header->ReadHeader(reader);

    header->SetAssetFilename(asset_file_path);
    header->SetEncryptionKey(encryption_key);

    if (status != Status::kSuccess) {
        return status;
    }

    headers_.push_back(std::move(header));

    return Status::kSuccess;
}

std::vector<std::string> SGAManager::GetFilenameList() const {
    std::vector<std::string> filenames;

    for (const auto& header : headers_) {
        const auto& entries = header->GetEntries();

        for (const auto& entry : entries) {
            filenames.push_back(entry.first);
        }
    }

    return filenames;
}

std::shared_ptr<SGAFile> SGAManager::GetFile(const std::string& filepath) {

    Header::Entry entry;

    std::filesystem::path path(filepath);
    auto nice_path = path.make_preferred().string();
    std::replace(nice_path.begin(), nice_path.end(), '\\', '/');

    auto success = false;

    Header* containing_header = nullptr;

    for (const auto& header : headers_) {
        if (header->GetEntry(entry, nice_path)) {
            success = true;
            containing_header = header.get();
            break;
        }
    }

    if (!success) {
        return nullptr;
    }

    Reader reader(containing_header->GetAssetFilename(), containing_header->GetEncryptionKey());

    auto header_size = containing_header->GetHeaderSize();
    auto offset = entry.offset;

    reader.JumpToPosition(header_size + offset);
    reader.PrepareSize(entry.file_size, entry.index);

    std::unique_ptr<uint8_t[]> data;

    reader.ReadData(data, entry.file_size);
    return std::make_shared<SGAFile>(std::move(data), entry.file_size);
}

} // namespace sga
