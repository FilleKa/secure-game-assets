#ifndef SGA_MANAGER_HPP_
#define SGA_MANAGER_HPP_

#include "header.hpp"
#include "status.hpp"

#include <memory>
#include <string>

namespace sga {

class SGAManager {
  public:
    Status OpenAssetFile(const std::string &asset_file_path,
                       const std::string &encryption_key);

    void ReadFile(const std::string &filename);

  private:
    std::unique_ptr<Header> header_;
    std::string asset_file_path_;
    std::string encryption_key_;
};

} // namespace sga

#endif /* SGA_MANAGER_HPP_ */
