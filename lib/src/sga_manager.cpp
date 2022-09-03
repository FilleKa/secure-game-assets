#include "sga_manager.hpp"
#include "reader.hpp"

#include "aes.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>

namespace sga {

Status SGAManager::OpenAssetFile(const std::string& asset_file_path,
                                 const std::string& encryption_key) {

    encryption_key_ = encryption_key;
    asset_file_path_ = asset_file_path;

    Reader reader(asset_file_path, encryption_key);

    if (!reader.IsFileOpen()) {
        return Status::kBadFile;
    }

    header_ = std::make_unique<Header>();
    auto status = header_->ReadHeader(reader);

    if (status != Status::kSuccess) {
        return status;
    }

    return Status::kSuccess;
}

std::shared_ptr<SGAFile> SGAManager::GetFile(const std::string& filepath) {

    Header::Entry entry;

    std::filesystem::path path(filepath);
    path.make_preferred();

    auto success = header_->GetEntry(entry, path.make_preferred().string());

    if (!success) {
        return nullptr;
    }

    Reader reader(asset_file_path_, encryption_key_);

    auto header_size = header_->GetHeaderSize();

    auto offset = entry.offset;
    reader.JumpToPosition(header_size + offset);
    reader.PrepareSize(entry.file_size, entry.index);

    std::unique_ptr<uint8_t[]> data;

    reader.ReadData(data, entry.file_size);
    return std::make_shared<SGAFile>(std::move(data), entry.file_size);
}

} // namespace sga
