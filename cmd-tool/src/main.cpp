
#include "log.hpp"
#include "packer.hpp"

int main(int argc, char** argv) {

    if (argc != 4) {
        sga::log::LogError("Bad usage. Please specify: input_folder output_file encryption_key");
        sga::log::LogInfo("Example usage:\n\t sga-packer.exe Resources/MyGameAssets packed-assets.sga secret_key");
        return -1;
    }

    std::string input_folder = argv[1];
    std::string output_file = argv[2];
    std::string encryption_key = argv[3];

    auto result = sga::Packer::Pack(input_folder, output_file, encryption_key);

    if (!result) {
        sga::log::LogError("Failed to pack assets.");
        return -1;
    }

    return 0;
}