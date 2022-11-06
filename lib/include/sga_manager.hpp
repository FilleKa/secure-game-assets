#ifndef SGA_MANAGER_HPP_
#define SGA_MANAGER_HPP_

#include "sga_file.hpp"
#include "sga_container.hpp"

#include "status.hpp"

#include <memory>
#include <string>

namespace sga {

class SGAManager {
  public:
    Status OpenAssetContainer(const std::string& asset_file_path,
                              const std::string& encryption_key,
                              bool keep_in_memory);

    std::shared_ptr<SGAFile> GetFile(const std::string& filepath);

    std::vector<std::string> GetFilenameList() const;

    std::vector<std::unique_ptr<SGAContainer>> containers_;
};

} // namespace sga

#endif /* SGA_MANAGER_HPP_ */
