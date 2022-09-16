
#include "log.hpp"
#include "packer.hpp"
#include "sga_manager.hpp"

int main(int argc, char** argv) {

    if (argc != 4 && argc != 3) {
        sga::log::LogError("Bad usage. Please specify: <input_folder> <output_file> [encryption_key]");
        sga::log::LogError("Or to list a container: list <container_file> [encryption_key]");
        sga::log::LogInfo("Example usage:\n\t sga-packer.exe Resources/MyGameAssets packed-assets.sga secret_key");
        return -1;
    }

    if (argv[1] == std::string("list")) {
        sga::SGAManager manager;
        std::string encryption_key = argc == 4 ? argv[3] : "";
        auto status = manager.OpenAssetContainer(argv[2], encryption_key, true);

        if (status != sga::Status::kSuccess) {
            sga::log::LogError("Failed to open file...");
            return -1;
        }

        auto filenames = manager.GetFilenameList();
        sga::log::LogInfo("Listing files:");
        for (const auto& filename : filenames) {
            sga::log::LogInfo(filename);
        }
        return 0;
    }

    std::string input_folder = argv[1];
    std::string output_file = argv[2];
    std::string encryption_key = argc == 4 ? argv[3] : "";

    auto result = sga::Packer::Pack(input_folder, output_file, encryption_key);

    if (!result) {
        sga::log::LogError("Failed to pack assets.");
        return -1;
    }

    return 0;
}