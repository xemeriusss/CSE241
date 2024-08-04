#include "fat12.h"
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fstream>

uint32_t getBlockSize(const std::string &fileName) {
    std::ifstream file(fileName, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Unable to open file system file.");
    }

    SuperBlock superBlock;
    file.read(reinterpret_cast<char*>(&superBlock), sizeof(SuperBlock));
    file.close();
    
    return superBlock.blockSize;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: fileSystemOper <file_system_data> <operation> [parameters...]" << std::endl;
        return 1;
    }

    std::string fileName = argv[1];
    std::string operation = argv[2];

    // FAT12FileSystem fs(BLOCK_SIZE_1024, fileName); 

    uint32_t blockSize;
    try {
        blockSize = getBlockSize(fileName);
    } catch (const std::exception &e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    FAT12FileSystem fs(blockSize, fileName);

    if (operation == "dir") {
        if (argc != 4) {
            std::cerr << "Usage: fileSystemOper <file_system_data> dir <path>" << std::endl;
            return 1;
        }
        std::string path = argv[3];
        fs.listDirectory(path);
    } 
    
    else if (operation == "mkdir") {
        if (argc != 4) {
            std::cerr << "Usage: fileSystemOper <file_system_data> mkdir <path>" << std::endl;
            return 1;
        }
        std::string path = argv[3];
        fs.makeDirectory(path);
    } 
    
    else if (operation == "rmdir") {
        if (argc != 4) {
            std::cerr << "Usage: fileSystemOper <file_system_data> rmdir <path>" << std::endl;
            return 1;
        }
        std::string path = argv[3];
        fs.removeDirectory(path);
    } 
    
    else if (operation == "dumpe2fs") {
        fs.dumpFileSystemInfo();
    } 
    
    else if (operation == "write") {
        if (argc != 5) {
            std::cerr << "Usage: fileSystemOper <file_system_data> write <path> <linux_file>" << std::endl;
            return 1;
        }
        std::string path = argv[3];
        std::string linuxFile = argv[4];
        fs.writeFile(path, linuxFile);
    } 
    
    else if (operation == "read") {
        if (argc != 5 && argc != 6) {
            std::cerr << "Usage: fileSystemOper <file_system_data> read <path> <linux_file> [password]" << std::endl;
            return 1;
        }
        std::string path = argv[3];
        std::string linuxFile = argv[4];
        std::string password = argc == 6 ? argv[5] : "";
        fs.readFile(path, linuxFile, password);
    } 
    
    else if (operation == "chmod") {
        if (argc != 5) {
            std::cerr << "Usage: fileSystemOper <file_system_data> chmod <path> <permissions>" << std::endl;
            return 1;
        }
        std::string path = argv[3];
        std::string permissions = argv[4];
        fs.changePermissions(path, permissions);
    } 
    
    else if (operation == "addpw") {
        if (argc != 5) {
            std::cerr << "Usage: fileSystemOper <file_system_data> addpw <path> <password>" << std::endl;
            return 1;
        }
        std::string path = argv[3];
        std::string password = argv[4];
        fs.addPassword(path, password);
    } 
    
    else if (operation == "del") {
        if (argc != 4) {
            std::cerr << "Usage: fileSystemOper <file_system_data> del <path>" << std::endl;
            return 1;
        }
        std::string path = argv[3];
        fs.deleteFile(path);
    }
    
    else {
        std::cerr << "Unknown operation: " << operation << std::endl;
        return 1;
    }

    return 0;
}
