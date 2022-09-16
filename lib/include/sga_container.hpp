#ifndef SGA_CONTAINER_HPP_
#define SGA_CONTAINER_HPP_

#include "header.hpp"
#include "sga_file.hpp"

#include <string>

namespace sga {

class SGAContainer {
  public:
    SGAContainer(const std::string& asset_file_path,
                 const std::string& encryption_key, bool keep_in_memory);

    const Header& GetHeader() const;

    Status GetStatus() const;
    
    std::shared_ptr<SGAFile> GetFile(const std::string& filepath);

  private:
    Header header_;
    Status status_;
    std::unique_ptr<Reader> reader_;
};
} // namespace sga

#endif /* SGA_CONTAINER_HPP_ */
