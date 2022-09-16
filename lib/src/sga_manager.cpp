#include "sga_manager.hpp"
#include "reader.hpp"

#include "aes.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace sga {

Status SGAManager::OpenAssetContainer(const std::string& asset_file_path,
                                      const std::string& encryption_key,
                                      bool keep_in_memory) {

    auto container = std::make_unique<SGAContainer>(
        asset_file_path, encryption_key, keep_in_memory);

    auto status = container->GetStatus();

    if (status == Status::kSuccess) {
        containers_.push_back(std::move(container));
    }

    return status;
}

std::vector<std::string> SGAManager::GetFilenameList() const {
    std::vector<std::string> filenames;

    for (const auto& container : containers_) {
        const auto& header = container->GetHeader();
        const auto& entries = header.GetEntries();

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

    for (const auto& container : containers_) {
        auto file = container->GetFile(nice_path);

        if (file) {
            return file;
        }
    }

    return nullptr;
}

} // namespace sga
