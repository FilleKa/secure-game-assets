
#include "sga_manager.hpp"

#include <iostream>

int main(int argc, char** argv) {

    sga::SGAManager manager;

    manager.OpenAssetFile("example/assets.sga", "secret_key");

    // Forward or backwards slashes don't matter..
    auto file_a = manager.GetFile("Resources\\a.txt");
    auto file_b = manager.GetFile("Resources/MoreResources\\b.txt");

    std::cout << "File a: \n" << file_a->GetPointer<char>() << std::endl;
    std::cout << "File b: \n" << file_b->GetPointer<char>() << std::endl;
}