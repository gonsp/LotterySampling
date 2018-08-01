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
        src/Main.cpp
        src/utils/InputParser.h
        src/utils/InputParser.cpp
        src/utils/Stats.h
        src/utils/Stats.cpp
        src/algorithms/GenericAlgorithm.h
        src/algorithms/lottery_sampling/Algorithm.h
        src/algorithms/lottery_sampling/Algorithm.cpp
        src/algorithms/lottery_sampling/Types.h
        src/algorithms/lottery_sampling/Types.cpp
        src/algorithms/space_saving/Algorithm.h
        src/algorithms/space_saving/Algorithm.cpp
        src/algorithms/space_saving/Types.h)



###################################################
# Executable
add_executable(k-hitting ${SOURCE_FILES})
target_include_directories(k-hitting PUBLIC src)




###################################################
# Tests
