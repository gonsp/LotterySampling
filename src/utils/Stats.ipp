#include <algorithms/GenericAlgorithm.h>
#include "Stats.h"

using namespace std::chrono;

Stats::Stats() {
    frequent_query_time = 0;
    frequent_query_count = 0;
    k_top_query_time = 0;
    k_top_query_count = 0;
    process_element_time = 0;
    process_element_count = 0;

    initial_time = high_resolution_clock::now();
}

counter Stats::get_interval(time_point from) {
    time_point to = high_resolution_clock::now();
    return duration_cast<nanoseconds>(to - from).count();
}

void Stats::start_counting(counter& counter) {
    start_time = high_resolution_clock::now();
    ++counter;
}

void Stats::finish_counting(counter& counter) {
    counter += get_interval(start_time);
}

template<class T>
void Stats::report(std::ostream& stream, GenericAlgorithmInterface<T>* algorithm) {
    // Python string format to build a dictionary
    stream << "{";
    stream << "'sample_size' : " << algorithm->sample_size() << ",";
    stream << "'memory_usage' : " << mstats().bytes_used << ",";
    stream << "'total_time' : " << get_interval(initial_time) << ",";
    stream << "'process_element_time' : " << process_element_time << ",";
    stream << "'process_element_count' : " << process_element_count << ",";
    stream << "'frequent_query_time' : " << frequent_query_time << ",";
    stream << "'frequent_query_count' : " << frequent_query_count << ",";
    stream << "'k_top_query_time' : " << k_top_query_time << ",";
    stream << "'k_top_query_count' : " << k_top_query_count;
    stream << "}" << std::endl;
}
