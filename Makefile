ifeq ($(BUILD_DIR),)
	BUILD_DIR := ./build
	_ = $(shell mkdir $(BUILD_DIR))
endif


all:
	g++ src/main.cpp -o $(BUILD_DIR)/Lottery_Sampling
