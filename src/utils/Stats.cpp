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

