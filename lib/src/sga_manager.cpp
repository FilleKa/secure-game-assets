#include "sga_manager.hpp"

#include <fstream>
#include <iostream>

namespace sga {

void SGAManager::OpenAssetFile(const std::string& asset_file_path) {

    std::ifstream is(asset_file_path, std::ios::in | std::ios::binary);

    if (!is.is_open()) {
        std::cout << "Failed to open file" << std::endl;
        return;
    }

    size_t entry_count = 0;

    is >> entry_count;

    std::cout << "Found " << entry_count << " entries" << std::endl;


}

} // namespace sga

