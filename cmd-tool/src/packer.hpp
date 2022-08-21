#ifndef SGA_PACKER_HPP_
#define SGA_PACKER_HPP_

#include <string>

namespace sga {

class Packer {
public:
    static bool Pack(const std::string& input_folder, const std::string& output_file, const std::string& encryption_key);

};

} // namespace sga

#endif /* SGA_PACKER_HPP_ */
