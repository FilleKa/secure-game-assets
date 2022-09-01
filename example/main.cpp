
#include "sga_manager.hpp"

int main(int argc, char** argv) {
    
    sga::SGAManager manager;
    
    manager.OpenAssetFile("example/assets.sga", "secret_key");
    
    manager.ReadFile("Resources/MoreResources/b.txt");
}