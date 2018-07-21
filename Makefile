##########################################
# DIRECTORIES
SRC_DIR = src
LIB_DIR =
BUILD_DIR = build
_ := $(shell mkdir -p $(BUILD_DIR))
##########################################

CFLAGS = -O0 -Wall -g
CPP = g++

all: k-hitting

k-hitting: $(SRC_DIR)/main.cpp $(BUILD_DIR)/LotterySampling.o $(BUILD_DIR)/SpaceSaving.o
	$(CPP) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/LotterySampling.o: $(SRC_DIR)/algorithms/lottery_sampling/LotterySampling.cpp $(SRC_DIR)/algorithms/Algorithm.h
	$(CPP) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/SpaceSaving.o: $(SRC_DIR)/algorithms/space_saving/SpaceSaving.cpp $(SRC_DIR)/algorithms/Algorithm.h
	$(CPP) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)