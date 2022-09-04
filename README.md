# Secure game assets

## Purpose
I quite often see people online asking the question ***"How do I protect my game
assets?"*** and a common answer I see is to use some *easy-to-crack caesar
cipher*, which is not ideal... This inspired me to create an *easy-to-use* tool
to solve this problem. The purpose of SGA is to prevent direct access and
modification of game assets. It could generally be any type of assets for
projects of any kind.

## How it works
SGA provides a *command line tool* to encrypt resource folders. All encrypted
files are combined into one binary file (**.sga**). Multiple asset files can be
generated to split data in a way that is logical to the target game/application.
For instance, the game assets could be split up into: *menu_assets.sga*,
*game_assets.sga*, *dlc_assets.sga* and so on... The command line tool can be
used in any *resource build step* to automatically add new resources to the
*sga* files. SGA also provides a small library to decrypt and read individual
resources files in the game. ***AES-CBC*** is used to encrypt the data using a
256 bit key. Each file will be encrypted to its own block so it is not required
to decrypt the whole asset file to retrieve one specific file. SGA uses a table
to know where each decrypted file starts & ends within the SGA file. This table
is also encrypted. 


## How to pack resources
Run the command line tool (preferably in your build step) as:
```sh
sga-packer.exe Resources/MyGameAssets assets.sga secret_key
```


## How to read resources
```cpp
#include "sga_manager.hpp"

int main(int argc, char** argv)  {
	sga::SGAManager manager;
	manager.OpenAssetFile("example/game_assets.sga", "secret_key");
	auto file_a = manager.GetFile("levels/level_1.json");
	// *file_a* can now provide pointer to data & file size.
}
```
The binary data is stored as *little-endian* and will be converted to
*big-endian* upon decryption IF required.
### Limitations
SGA is not yet thread safe and would be my next priority, unless other bugs pop
up :)  It would also be nice to compress the data. Altough it's possible
compress/decompress the *sga* files manually with any compression library.

### CMake guide for lazy people ;)
```sh
cd secure-game-assets
mkdir build
cd build
cmake ..
cmake --build .
#to run tests
test/sga-test
```
This should create the command line tool in **build/cmd-tool** and library files
in **build/lib**. Then you just need to include the headers located in
**secure-game-assets/lib/include**. Typically you should include
**sga_manager.hpp**.