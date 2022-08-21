#ifndef SGA_MANAGER_HPP_
#define SGA_MANAGER_HPP_

#include "header.hpp"

#include <string>
#include <memory>

namespace sga {

class SGAManager {
public:

    void OpenAssetFile(const std::string& asset_file_path);

    void ReadFile(const std::string& filename);

private:

    std::unique_ptr<Header> header_;
    std::string asset_file_path_;
};

} // namespace sga

#endif /* SGA_MANAGER_HPP_ */
