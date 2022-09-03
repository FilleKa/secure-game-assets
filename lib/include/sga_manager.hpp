#ifndef SGA_MANAGER_HPP_
#define SGA_MANAGER_HPP_

#include "header.hpp"
#include "sga_file.hpp"
#include "status.hpp"

#include <memory>
#include <string>

namespace sga {

class SGAManager {
  public:
    Status OpenAssetFile(const std::string& asset_file_path,
                         const std::string& encryption_key);

    std::shared_ptr<SGAFile> GetFile(const std::string& filepath);

  private:
    std::unique_ptr<Header> header_;
    std::string asset_file_path_;
    std::string encryption_key_;
};

} // namespace sga

#endif /* SGA_MANAGER_HPP_ */
