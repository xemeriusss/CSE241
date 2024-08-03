# Makefile

# Compiler
CC = gcc

# Compiler flags
CFLAGS = -pthread -lm

# Target binaries
TARGETS = PideShop HungryVeryMuch

# Source files
SRC_PideShop = PideShop.c
SRC_HungryVeryMuch = HungryVeryMuch.c

all: $(TARGETS)

PideShop: $(SRC_PideShop)
	$(CC) -o $@ $^ $(CFLAGS)

HungryVeryMuch: $(SRC_HungryVeryMuch)
	$(CC) -o $@ $^

clean:
	rm -f $(TARGETS) *.o
