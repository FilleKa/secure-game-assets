#include "sga_manager.hpp"

int main(int arg, char** argv) {

    sga::SGAManager mng;

    mng.OpenAssetFile("asd");

    mng.ReadFile("Resources/a.txt");
    mng.ReadFile("Resources/MoreResources/b.txt");

    return 0;
}