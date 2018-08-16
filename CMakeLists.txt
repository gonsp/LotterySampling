cmake_minimum_required(VERSION 3.10)
project(Lottery_Sampling)


###################################################
# Compiler settings
set(CMAKE_CXX_STANDARD 11)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif(NOT CMAKE_BUILD_TYPE)

set(CMAKE_CXX_FLAGS_RELEASEWITHASSERTS "-Wall -O3")


###################################################
# Source files
set(SOURCE_FILES
        src/Main.cpp
        src/utils/InputParser.h
        src/utils/InputParser.cpp
        src/utils/Stats.h
        src/utils/Stats.ipp
        src/utils/Misc.h
        src/algorithms/GenericAlgorithm.h
        src/algorithms/GenericAlgorithm.ipp
        src/algorithms/lottery_sampling/Algorithm.h
        src/algorithms/lottery_sampling/Algorithm.ipp
        src/algorithms/lottery_sampling/Types.h
        src/algorithms/space_saving/Algorithm.h
        src/algorithms/space_saving/Algorithm.ipp
        src/algorithms/space_saving/Types.h
        src/data_structures/TicketOrder.h
        src/data_structures/TicketOrder.ipp
        src/data_structures/SortedTree.h
        src/data_structures/SortedTree.ipp
        src/data_structures/SortedList.h
        src/data_structures/SortedList.ipp)


###################################################
# Executable
add_executable(heavy_hitters ${SOURCE_FILES})
target_include_directories(heavy_hitters PUBLIC src)


###################################################
# Tests
enable_testing()

add_test(
        NAME
        MemoryLeak
        COMMAND
        python3 main.py MemoryLeak
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/test)

#add_test(
#        NAME
#        MemoryUsage
#        COMMAND
#        python3 main.py MemoryUsage -m 1000 -N 100000 -stream zipf
#        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/test)

add_test(
        NAME
        MemoryUsageAsymptotic
        COMMAND
        python3 main.py MemoryUsageAsymptotic -m 1000 -N 100000 -stream zipf
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/test)

add_test(
        NAME
        MemoryUsageEvolution
        COMMAND
        python3 main.py MemoryUsageEvolution -m 1000 -N 1000000 -stream zipf -sample_period 1000
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/test)

