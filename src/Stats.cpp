#include "Stats.h"

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