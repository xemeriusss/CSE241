#include "fat12.h"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <vector>
#include <cstring>
#include <ctime>
#include <algorithm>
#include <sstream>
#include <iomanip>

// Constructor
FAT12FileSystem::FAT12FileSystem(uint32_t blockSize, const std::string &fileName)
    : blockSize(blockSize), fileName(fileName) {
    loadSuperBlock();
    loadFAT();
    loadDirectoryTable();
}

// Destructor
FAT12FileSystem::~FAT12FileSystem() {
    saveDirectoryTable();
    saveSuperBlock();
    saveFAT();
}

// std::string FAT12FileSystem::getCurrentDateTime() {
//     std::time_t now = std::time(nullptr);
//     std::tm *ltm = std::localtime(&now);
//     std::ostringstream oss;
//     oss << std::put_time(ltm, "%Y%m%d%H%M%S");
//     return oss.str();
// }

void FAT12FileSystem::initializeFileSystem() {
    std::ofstream file(fileName, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Unable to create file system file.");
    }

    // Initialize the file system with zeros
    std::vector<char> buffer(MAX_FILE_SYSTEM_SIZE, 0);
    file.write(buffer.data(), buffer.size());
    file.close();

    // Initialize super block
    superBlock.blockSize = blockSize;
    superBlock.totalBlocks = MAX_FILE_SYSTEM_SIZE / blockSize;
    superBlock.freeBlocks = superBlock.totalBlocks - 1; // 1 block for root directory
    superBlock.rootDirectoryBlock = 1;

    std::cout << "Block number: " << superBlock.totalBlocks << std::endl;

    // Initialize FAT table
    fatTable.resize(superBlock.totalBlocks, FAT_FREE);
    fatTable[0] = FAT_END; // Root directory block

    // Initialize root directory
    DirectoryEntry rootDir;
    strncpy(rootDir.fileName, "\\", sizeof(rootDir.fileName) - 1);
    rootDir.fileName[sizeof(rootDir.fileName) - 1] = '\0';
    rootDir.size = 0;
    rootDir.permissions = 0; // RW permissions
    rootDir.creationDate = getCurrentDateTimeBits();
    rootDir.modificationDate = getCurrentDateTimeBits();
    rootDir.password[0] = '\0';
    rootDir.is_directory = true;
    rootDir.startBlock = 0;

    directoryTable.push_back(rootDir);

    //std::cout << "File system created successfully!" << std::endl;
}

void FAT12FileSystem::createFileSystem() {
    initializeFileSystem();
    saveSuperBlock();
    saveFAT();
    saveDirectoryTable();
}

bool FAT12FileSystem::directoryExists(const std::string &path) {
    if (path == "\\") {
        return true;
    }
    for (const auto &entry : directoryTable) {
        if (std::string(entry.fileName) == path && entry.is_directory) {
            return true;
        }
    }
    return false;
}

void FAT12FileSystem::makeDirectory(const std::string &path) {
    if (directoryExists(path)) {
        std::cerr << "Directory already exists: " << path << std::endl;
        return;
    }
    
    std::size_t pos = path.find_last_of("\\/"); // Find the last directory separator
    if (pos != std::string::npos) {
        std::string parentPath = path.substr(0, pos);
        if (parentPath.empty()) {
            parentPath = "\\"; // Handle root directory as the parent
        }
        if (!directoryExists(parentPath)) {
            std::cerr << "Parent directory does not exist: " << parentPath << std::endl;
            return;
        }
    }
    
    DirectoryEntry newDir;
    strncpy(newDir.fileName, path.c_str(), sizeof(newDir.fileName) - 1);
    newDir.fileName[sizeof(newDir.fileName) - 1] = '\0';
    newDir.size = 0;
    newDir.permissions = READ_PERMISSION | WRITE_PERMISSION; // Default RW permissions

    newDir.creationDate = getCurrentDateTimeBits();
    newDir.modificationDate = getCurrentDateTimeBits();

    newDir.password[0] = '\0';
    newDir.is_directory = true;
    newDir.startBlock = allocateBlock();

    directoryTable.push_back(newDir);
    std::cout << "Directory created: " << path << std::endl;
    saveDirectoryTable();
    saveFAT();
}

void FAT12FileSystem::removeDirectory(const std::string &path) {
    auto it = std::remove_if(directoryTable.begin(), directoryTable.end(),
                             [&path](const DirectoryEntry &entry) {
                                 return std::string(entry.fileName).find(path) == 0;
                             });
    if (it != directoryTable.end()) {
        for (auto iter = it; iter != directoryTable.end(); ++iter) {
            deallocateBlock(iter->startBlock);
        }
        directoryTable.erase(it, directoryTable.end());
        std::cout << "Directory removed: " << path << std::endl;
        saveDirectoryTable();
        saveFAT();
    } else {
        std::cerr << "Directory not found: " << path << std::endl;
    }
}

void FAT12FileSystem::dumpFileSystemInfo() {
    std::cout << "File system information for: " << fileName << std::endl;
    std::cout << "Block size: " << superBlock.blockSize << " bytes" << std::endl;
    std::cout << "Total blocks: " << superBlock.totalBlocks << std::endl;
    std::cout << "Free blocks: " << superBlock.freeBlocks << std::endl;
    
    // Number of directories and files
    size_t dirCount = 0;
    size_t fileCount = 0;
    for (const auto &entry : directoryTable) {
        if (entry.is_directory) {
            ++dirCount;
        } else {
            if(entry.fileName[0] != '\0') {
                ++fileCount;
            }
        }
    }


    std::cout << "Total directories: " << dirCount << std::endl;
    std::cout << "Total files: " << fileCount << std::endl;
    for (const auto &entry : directoryTable) {
        if(entry.fileName[0] != '\0') {
            std::cout << "Name: " << entry.fileName << ", Size: " << entry.size
                  << ", Permissions: " << permissionsToString(entry.permissions)
                  << ", Creation Date: " << formatDateTime(entry.creationDate)
                  << ", Modification Date: " << formatDateTime(entry.modificationDate)
                  << ", Password Protected: " << (strlen(entry.password) > 0 ? "Yes" : "No")
                  << ", Type: " << (entry.is_directory ? "Directory" : "File")
                  << ", Start Block: " << entry.startBlock << std::endl;
        }
    }
}

void FAT12FileSystem::writeFile(const std::string &path, const std::string &linuxFile) {
    std::ifstream inFile(linuxFile, std::ios::binary);
    if (!inFile) {
        throw std::runtime_error("Unable to open Linux file for reading.");
    }

    DirectoryEntry newFile;
    strncpy(newFile.fileName, path.c_str(), sizeof(newFile.fileName) - 1);
    newFile.fileName[sizeof(newFile.fileName) - 1] = '\0';

    newFile.creationDate = getCurrentDateTimeBits();
    newFile.modificationDate = getCurrentDateTimeBits();

    // // Check write permission
    // if(!(newFile.permissions & WRITE_PERMISSION)) {
    //     std::cerr << "No write permission for: " << path << std::endl;
    //     return;
    // }

    //newFile.permissions = 0; // RW permissions
    newFile.permissions = READ_PERMISSION | WRITE_PERMISSION; // Default RW permissions

    newFile.password[0] = '\0';
    newFile.is_directory = false;
    newFile.startBlock = allocateBlock();

    inFile.seekg(0, std::ios::end);
    newFile.size = inFile.tellg();
    inFile.seekg(0, std::ios::beg);

    std::vector<char> buffer(newFile.size);
    inFile.read(buffer.data(), buffer.size());
    inFile.close();

    std::ofstream outFile(fileName, std::ios::binary | std::ios::in | std::ios::out);
    if (!outFile) {
        throw std::runtime_error("Unable to open file system file for writing.");
    }

    uint16_t currentBlock = newFile.startBlock;
    uint32_t bytesRemaining = newFile.size;
    uint32_t offset = 0;

    std::cout << "startBlock: " << newFile.startBlock << std::endl;
    std::cout << "size: " << newFile.size << std::endl;

    while (bytesRemaining > 0) {
        uint32_t bytesToWrite = std::min(bytesRemaining, blockSize);
        outFile.seekp(currentBlock * blockSize);
        outFile.write(buffer.data() + offset, bytesToWrite);

        offset += bytesToWrite;
        bytesRemaining -= bytesToWrite;

        if (bytesRemaining > 0) {
            uint16_t nextBlock = allocateBlock();
            fatTable[currentBlock] = nextBlock;
            currentBlock = nextBlock;
        } else {
            fatTable[currentBlock] = FAT_END;
        }
    }
    outFile.close();

    directoryTable.push_back(newFile);
    std::cout << "File written to file system: " << path << std::endl;
    //std::cout << "File contents: " << std::endl;    
    //std::cout << buffer.data() << std::endl;
    saveDirectoryTable();
    saveFAT();

    //printFatTable();

}


void FAT12FileSystem::deleteFile(const std::string &path) {
    auto it = std::find_if(directoryTable.begin(), directoryTable.end(),
                           [&path](const DirectoryEntry &entry) {
                               return std::string(entry.fileName) == path && !entry.is_directory;
                           });
    if (it != directoryTable.end()) {
        uint16_t currentBlock = it->startBlock;
        while (currentBlock != FAT_END) {
            uint16_t nextBlock = fatTable[currentBlock];
            fatTable[currentBlock] = FAT_FREE;
            currentBlock = nextBlock;
        }
        directoryTable.erase(it);
        std::cout << "File deleted: " << path << std::endl;
        saveDirectoryTable();
        saveFAT();

        // Update free blocks in super block
        //superBlock.freeBlocks += it->size / blockSize + 1;

    } else {
        std::cerr << "File not found: " << path << std::endl;
    }
}

void FAT12FileSystem::readFile(const std::string &path, const std::string &linuxFile, const std::string &password) {
    auto it = std::find_if(directoryTable.begin(), directoryTable.end(),
                           [&path](const DirectoryEntry &entry) {
                               return std::string(entry.fileName) == path;
                           });

    if (it == directoryTable.end()) {
        throw std::runtime_error("File not found.");
    }

    DirectoryEntry &fileEntry = *it;
    if (fileEntry.is_directory) {
        throw std::runtime_error("Path is a directory.");
    }

    // Check password
    if(strlen(fileEntry.password) > 0 && password != fileEntry.password) {
        std::cerr << "Incorrect password for: " << path << std::endl;
        return;
    }

    if (!(it->permissions & READ_PERMISSION)) {
        std::cerr << "No read permission for: " << path << std::endl;
        return;
    }

    std::ofstream outFile(linuxFile, std::ios::binary);
    if (!outFile) {
        throw std::runtime_error("Unable to open Linux file for writing.");
    }

    std::ifstream inFile(fileName, std::ios::binary);
    if (!inFile) {
        throw std::runtime_error("Unable to open file system file for reading.");
    }

    uint16_t currentBlock = fileEntry.startBlock;
    uint32_t bytesRemaining = fileEntry.size;
    std::vector<char> buffer(blockSize);

    std::cout << "startBlock in read: " << fileEntry.startBlock << std::endl;
    std::cout << "size in read: " << fileEntry.size << std::endl;

    while (bytesRemaining > 0) {
        uint32_t bytesToRead = std::min(bytesRemaining, blockSize);
        //std::cout << "bytesToRead in read: " << bytesToRead << std::endl;
        inFile.seekg(currentBlock * blockSize);
        inFile.read(buffer.data(), bytesToRead);

        // std::cout << "Contents: " << std::endl;
        // std::cout << buffer.data() << std::endl;
        
        outFile.write(buffer.data(), bytesToRead);

        bytesRemaining -= bytesToRead;
        currentBlock = fatTable[currentBlock];

        
        
        if (currentBlock == FAT_END) break;
    }


    inFile.close();
    outFile.close();
    std::cout << "File read from file system to Linux file: " << linuxFile << std::endl;
}



void FAT12FileSystem::saveSuperBlock() {
    std::ofstream file(fileName, std::ios::binary | std::ios::in | std::ios::out);
    if (!file) {
        throw std::runtime_error("Unable to open file system file for writing.");
    }

    file.seekp(0);
    file.write(reinterpret_cast<const char *>(&superBlock), sizeof(superBlock));
    file.close();
}

void FAT12FileSystem::loadSuperBlock() {
    std::ifstream file(fileName, std::ios::binary);
    if (!file) {
        initializeFileSystem();
        saveSuperBlock();
    } else {
        file.read(reinterpret_cast<char *>(&superBlock), sizeof(superBlock));
        file.close();
    }
}

void FAT12FileSystem::saveFAT() {
    std::ofstream file(fileName, std::ios::binary | std::ios::in | std::ios::out);
    if (!file) {
        throw std::runtime_error("Unable to open file system file for writing.");
    }

    // Write FAT after the superblock
    file.seekp(sizeof(SuperBlock));
    file.write(reinterpret_cast<const char *>(fatTable.data()), fatTable.size() * sizeof(uint16_t));
    file.close();
}

void FAT12FileSystem::loadFAT() {
    std::ifstream file(fileName, std::ios::binary);
    if (!file) {
        initializeFileSystem();
        saveFAT();
    } else {
        fatTable.resize(superBlock.totalBlocks);
        file.seekg(sizeof(SuperBlock));
        file.read(reinterpret_cast<char *>(fatTable.data()), fatTable.size() * sizeof(uint16_t));
        file.close();
    }
}

void FAT12FileSystem::saveDirectoryTable() {
    std::ofstream file(fileName, std::ios::binary | std::ios::in | std::ios::out);
    if (!file) {
        throw std::runtime_error("Unable to open file system file for writing.");
    }

    // Write directory table after the FAT
    file.seekp(sizeof(SuperBlock) + fatTable.size() * sizeof(uint16_t));
    for (const auto &entry : directoryTable) {
        file.write(reinterpret_cast<const char *>(&entry), sizeof(entry));
    }
    file.close();
}

void FAT12FileSystem::loadDirectoryTable() {
    std::ifstream file(fileName, std::ios::binary);
    if (!file) {
        initializeFileSystem();
        saveDirectoryTable();
    } else {
        file.seekg(sizeof(SuperBlock) + fatTable.size() * sizeof(uint16_t));
        DirectoryEntry entry;
        while (file.read(reinterpret_cast<char *>(&entry), sizeof(entry))) {
            directoryTable.push_back(entry);
        }
        file.close();
    }
}

uint16_t FAT12FileSystem::allocateBlock() {
    for (uint16_t i = 1; i < fatTable.size(); ++i) { // Start from 1 to skip the reserved block
        if (fatTable[i] == FAT_FREE) {
            fatTable[i] = FAT_END;
            superBlock.freeBlocks--;
            return i;
        }
    }
    throw std::runtime_error("No free blocks available.");
}

void FAT12FileSystem::deallocateBlock(uint16_t block) {
    uint16_t currentBlock = block;
    while (currentBlock != FAT_END) {
        uint16_t nextBlock = fatTable[currentBlock];
        fatTable[currentBlock] = FAT_FREE;
        currentBlock = nextBlock;
        superBlock.freeBlocks++;
    }
}

void FAT12FileSystem::listDirectory(const std::string &path) {
    if (!directoryExists(path)) {
        std::cerr << "Directory does not exist: " << path << std::endl;
        return;
    }

    std::cout << "Listing contents of directory: " << path << std::endl;
    for (const auto &entry : directoryTable) {
        std::string entryPath = entry.fileName;
        std::string parentPath = entryPath.substr(0, entryPath.find_last_of("\\/"));

        // Handle root directory
        if (parentPath.empty()) {
            parentPath = "\\";
        } 

        // Handle root directory as the parent
        if (parentPath == path && entryPath != "\\") {

            if(entry.fileName[0] != '\0') {
                if (entry.is_directory) {
                    std::cout << "Directory: " << entry.fileName << std::endl;
                } else {
                    std::cout << "File: " << entry.fileName << std::endl;
                }
            }
        }
    }

}

//Prints the FAT table
void FAT12FileSystem::printFatTable() {
    std::cout << "FAT Table:" << std::endl;
    for (size_t i = 0; i < fatTable.size(); ++i) {
        std::cout << i << ": " << fatTable[i] << std::endl;
    }
}

void FAT12FileSystem::changePermissions(const std::string &path, const std::string &permissions) {
    auto it = std::find_if(directoryTable.begin(), directoryTable.end(),
                           [&path](const DirectoryEntry &entry) {
                               return std::string(entry.fileName) == path;
                           });
    if (it == directoryTable.end()) {
        throw std::runtime_error("File or directory not found: " + path);
    }

    if (permissions[0] == '+') {
        for (size_t i = 1; i < permissions.size(); ++i) {
            if (permissions[i] == 'r') {
                it->permissions |= READ_PERMISSION;
            } else if (permissions[i] == 'w') {
                it->permissions |= WRITE_PERMISSION;
            }
        }
    } else if (permissions[0] == '-') {
        for (size_t i = 1; i < permissions.size(); ++i) {
            if (permissions[i] == 'r') {
                it->permissions &= ~READ_PERMISSION;
            } else if (permissions[i] == 'w') {
                it->permissions &= ~WRITE_PERMISSION;
            }
        }
    } else {
        throw std::runtime_error("Invalid permissions format: " + permissions);
    }

    saveDirectoryTable();
    std::cout << "Permissions changed for: " << path << std::endl;
}

std::string FAT12FileSystem::permissionsToString(uint16_t permissions) {
    std::string result;
    if (permissions & READ_PERMISSION) {
        result += 'r';
    } else {
        result += '-';
    }
    if (permissions & WRITE_PERMISSION) {
        result += 'w';
    } else {
        result += '-';
    }
    return result;
}

void FAT12FileSystem::addPassword(const std::string &path, const std::string &password) {
    auto it = std::find_if(directoryTable.begin(), directoryTable.end(),
                           [&path](const DirectoryEntry &entry) {
                               return std::string(entry.fileName) == path;
                           });
    if (it == directoryTable.end()) {
        throw std::runtime_error("File or directory not found: " + path);
    }

    strncpy(it->password, password.c_str(), sizeof(it->password) - 1);
    it->password[sizeof(it->password) - 1] = '\0';

    saveDirectoryTable();
    std::cout << "Password added to: " + path << std::endl;
}

uint32_t FAT12FileSystem::getCurrentDateTimeBits() {
    std::time_t now = std::time(nullptr);
    std::tm *ltm = std::localtime(&now);

    uint32_t dateTime = 0;
    dateTime |= ((ltm->tm_year - 100) & 0x7F) << 25; // Year (2000-2127)
    dateTime |= (ltm->tm_mon + 1) << 21; // Month (1-12)
    dateTime |= ltm->tm_mday << 16; // Day (1-31)
    dateTime |= ltm->tm_hour << 11; // Hour (0-23)
    dateTime |= ltm->tm_min << 5; // Minute (0-59)
    dateTime |= (ltm->tm_sec / 2); // Second (0-30, 2-second intervals)

    return dateTime;
}

std::string FAT12FileSystem::formatDateTime(uint32_t dateTime) {
    std::ostringstream oss;
    oss << ((dateTime >> 25) & 0x7F) + 2000 << "-"  // Year
        << std::setw(2) << std::setfill('0') << ((dateTime >> 21) & 0x0F) << "-"  // Month
        << std::setw(2) << std::setfill('0') << ((dateTime >> 16) & 0x1F) << " "  // Day
        << std::setw(2) << std::setfill('0') << ((dateTime >> 11) & 0x1F) << ":"  // Hour
        << std::setw(2) << std::setfill('0') << ((dateTime >> 5) & 0x3F) << ":"  // Minute
        << std::setw(2) << std::setfill('0') << ((dateTime & 0x1F) * 2);  // Second

    return oss.str();
}



