#include <algorithms/GenericAlgorithm.h>
#include "Stats.h"
#include <malloc/malloc.h>

using namespace std::chrono;

Stats::Stats() {
    frequent_query_time = 0;
    frequent_query_count = 0;
    k_top_query_time = 0;
    k_top_query_count = 0;
    process_element_time = 0;
    process_element_count = 0;
}

void Stats::start_counting(counter& counter) {
    start_time = high_resolution_clock::now();
    ++counter;
}

void Stats::finish_counting(counter& counter) {
    high_resolution_clock::time_point end_time = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end_time - start_time).count();
    counter += duration;
}

void Stats::report(std::ostream& stream, GenericAlgorithmInterface* algorithm) {
    // Python string format to build a dictionary
    stream << "{";
    stream << "'sample_size' : " << algorithm->sample_size() << ",";
    stream << "'memory_usage' : " << mstats().bytes_used << ",";
    stream << "'frequent_query_time' : " << frequent_query_time << ",";
    stream << "'frequent_query_count' : " << frequent_query_count << ",";
    stream << "'k_top_query_time' : " << k_top_query_time << ",";
    stream << "'k_top_query_count' : " << k_top_query_count << ",";
    stream << "'process_element_time' : " << process_element_time << ",";
    stream << "'process_element_count' : " << process_element_count;
    stream << "}" << std::endl;
}

