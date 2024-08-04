#ifndef FAT12_H
#define FAT12_H

#include <ctime>
#include <string>
#include <vector>
#include <cstdint>

#define BLOCK_SIZE_512 512
#define BLOCK_SIZE_1024 1024
#define MAX_FILE_SYSTEM_SIZE (4 * 1024 * 1024) // 4 MB
#define FAT_FREE 0x0000
#define FAT_END 0xFFFF

#define READ_PERMISSION 0x01
#define WRITE_PERMISSION 0x02

struct SuperBlock {
    uint32_t blockSize;
    uint32_t totalBlocks;
    uint32_t freeBlocks;
    uint32_t rootDirectoryBlock;
};

struct DirectoryEntry {
    char fileName[25]; // Fixed length
    uint32_t size; // Size of the file in bytes
    uint8_t permissions; // Read and write permissions
    uint32_t creationDate; // Date in bits
    uint32_t modificationDate; // Date in bits
    char password[5]; // Password for protected files (empty if not protected)
    bool is_directory; // True if the entry is a directory, false if it's a file
    uint16_t startBlock; // Start block of the file or directory
};

class FAT12FileSystem {
public:
    FAT12FileSystem(uint32_t blockSize, const std::string &fileName);
    ~FAT12FileSystem();
    
    void createFileSystem();
    void listDirectory(const std::string &path);
    void makeDirectory(const std::string &path);
    void removeDirectory(const std::string &path);
    void dumpFileSystemInfo();
    void writeFile(const std::string &path, const std::string &linuxFile);
    void readFile(const std::string &path, const std::string &linuxFile, const std::string &password = "");
    void deleteFile(const std::string &path);
    void changePermissions(const std::string &path, const std::string &permissions);
    void addPassword(const std::string &path, const std::string &password);

private:
    uint32_t blockSize;
    std::string fileName;
    SuperBlock superBlock;
    std::vector<DirectoryEntry> directoryTable;
    std::vector<uint16_t> fatTable;
    
    void initializeFileSystem();
    void saveDirectoryTable();
    void loadDirectoryTable();
    void saveSuperBlock();
    void loadSuperBlock();
    void saveFAT();
    void loadFAT();
    uint16_t allocateBlock();
    void deallocateBlock(uint16_t block);
    void printFatTable();
    bool directoryExists(const std::string &path);

    //std::string getCurrentDateTime();
    std::string permissionsToString(uint16_t permissions);
    uint32_t getCurrentDateTimeBits();
    std::string formatDateTime(uint32_t dateTime);
};

#endif // FAT12_H
