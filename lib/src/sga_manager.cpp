#include "sga_manager.hpp"
#include "reader.hpp"

#include "aes.hpp"

#include <fstream>
#include <iostream>

namespace sga {

Status SGAManager::OpenAssetFile(const std::string &asset_file_path,
                                 const std::string &encryption_key) {

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

void SGAManager::ReadFile(const std::string &filename) {

    Header::Entry entry;

    auto success = header_->GetEntry(entry, filename);

    if (!success) {
        return;
    }

    Reader reader(asset_file_path_, encryption_key_);

    auto header_size = header_->GetHeaderSize();

    auto offset = entry.offset;
    reader.JumpToPosition(header_size + offset);
    reader.PrepareSize(entry.file_size, entry.index);

    std::string data;
    reader.ReadString(data, entry.file_size);

    std::cout << "Data: " << data << std::endl;
}

} // namespace sga
