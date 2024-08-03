#include "fat12.h"
#include <iostream>
#include <cstdlib>

int main(int argc, char *argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: makeFileSystem <block_size_in_KB> <file_system_name>" << std::endl;
        return 1;
    }

    double blockSizeKB = std::stod(argv[1]);
    if (blockSizeKB != 0.5 && blockSizeKB != 1) {
        std::cerr << "Block size must be 0.5 or 1 KB" << std::endl;
        return 1;
    }
    uint32_t blockSize = static_cast<uint32_t>(blockSizeKB * 1024); // Convert to bytes
    std::string fileName = argv[2];

    FAT12FileSystem fs(blockSize, fileName);
    fs.createFileSystem();

    std::cout << "File system created successfully!" << std::endl;
    return 0;
}
