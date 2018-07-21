cmake_minimum_required(VERSION 3.10)
project(Lottery_Sampling)

###################################################
# Compiler settings
set(CMAKE_CXX_STANDARD 11)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE ReleaseWithAsserts)
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_FLAGS_RELEASEWITHASSERTS "-Wall -O3")


###################################################
# Source files
set(SOURCE_FILES
        src/main.cpp
        src/InputParser.h
        src/InputParser.cpp
        src/algorithms/Algorithm.h
        src/algorithms/Algorithm.cpp
        src/algorithms/lottery_sampling/LotterySampling.h
        src/algorithms/lottery_sampling/LotterySampling.cpp
        src/algorithms/space_saving/SpaceSaving.h
        src/algorithms/space_saving/SpaceSaving.cpp
        )


###################################################
# Executable
add_executable(k-hitting ${SOURCE_FILES})




###################################################
# Tests
