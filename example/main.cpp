
#include "sga_manager.hpp"

#include <iostream>

int main(int argc, char** argv) {

    sga::SGAManager manager;

    manager.OpenAssetContainer("example/menu_assets.sga", "secret_key", false);
    manager.OpenAssetContainer("example/game_assets.sga", "secret_key", true);

    std::cout << "List of all decrypted files:" << std::endl;
    auto filenames = manager.GetFilenameList();
    for (const auto& filename : filenames) {
        std::cout << filename << std::endl;
    }

    // Forward or backwards slashes don't matter..
    auto file_a = manager.GetFile("game_assets\\a.txt");
    auto file_b = manager.GetFile("game_assets/subfolder\\b.txt");

    auto menu_file = manager.GetFile("menu_assets/menu_file.txt");

    std::cout << "game file a: \n" << file_a->GetPointer<char>() << std::endl;
    std::cout << "game file b: \n" << file_b->GetPointer<char>() << std::endl;
    std::cout << "menu file: \n" << menu_file->GetPointer<char>() << std::endl;
}