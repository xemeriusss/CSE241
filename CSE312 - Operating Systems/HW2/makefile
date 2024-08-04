# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -std=c++11

# Targets
TARGETS = makeFileSystem fileSystemOper

# Source files
SOURCES = fat12.cpp
MAKE_FILE_SYSTEM_SOURCES = makeFileSystem.cpp $(SOURCES)
FILE_SYSTEM_OPER_SOURCES = fileSystemOper.cpp $(SOURCES)

# Default target
all: $(TARGETS)

# Compile makeFileSystem
makeFileSystem: $(MAKE_FILE_SYSTEM_SOURCES)
	$(CXX) $(CXXFLAGS) -o makeFileSystem $(MAKE_FILE_SYSTEM_SOURCES)

# Compile fileSystemOper
fileSystemOper: $(FILE_SYSTEM_OPER_SOURCES)
	$(CXX) $(CXXFLAGS) -o fileSystemOper $(FILE_SYSTEM_OPER_SOURCES)

# Clean up
clean:
	rm -f $(TARGETS) *.o

.PHONY: all clean
