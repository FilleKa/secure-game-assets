#include "sga_manager.hpp"
#include "reader.hpp"

#include "aes.hpp"

#include <fstream>
#include <iostream>


namespace sga {

void SGAManager::OpenAssetFile(const std::string& asset_file_path) {

    asset_file_path_ = asset_file_path;

    sga::Reader reader(asset_file_path, "asd");

    if (!reader.IsOpen()) {
        std::cout << "Failed to open file: " << asset_file_path << std::endl;
        return;
    }

    header_ = std::make_unique<Header>();
    header_->ReadHeader(reader);

}


void SGAManager::ReadFile(const std::string& filename) {

    Header::Entry entry;

    auto success = header_->GetEntry(entry, filename);

    if (!success) {
        return;
    }

    Reader reader(asset_file_path_, "asd");

    auto header_size = header_->GetHeaderSize();

    auto offset = entry.offset;
    reader.JumpToPosition(header_size + offset);
    reader.PrepareSize(entry.file_size);
    auto data = reader.ReadString(entry.file_size);

    std::cout << "Data: " << data << std::endl;
}

} // namespace sga

