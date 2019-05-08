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

Stats::counter Stats::get_interval(time_point from) {
    time_point to = high_resolution_clock::now();
    return duration_cast<nanoseconds>(to - from).count();
}

void Stats::start_counting() {
    start_time = high_resolution_clock::now();
}

void Stats::finish_counting(counter& counter) {
    counter += get_interval(start_time);
}

#if __APPLE__
#include <malloc/malloc.h>
#endif

template<class T>
void Stats::report(std::ostream& stream, GenericAlgorithmInterface<T>* algorithm) {
    // Python string format to build a dictionary
    stream << "{";
    stream << "'sample_size' : " << algorithm->sample_size() << ",";
    stream << "'threshold' : " << algorithm->get_threshold() << ",";
#if __APPLE__
    stream << "'memory_usage' : " << mstats().bytes_used << ",";
#else
    stream << "'memory_usage' : " << -1 << ",";
#endif
    stream << "'total_time' : " << get_interval(initial_time) << ",";
    stream << "'process_element_time' : " << process_element_time << ",";
    stream << "'process_element_count' : " << process_element_count << ",";
    stream << "'frequent_query_time' : " << frequent_query_time << ",";
    stream << "'frequent_query_count' : " << frequent_query_count << ",";
    stream << "'k_top_query_time' : " << k_top_query_time << ",";
    stream << "'k_top_query_count' : " << k_top_query_count;
    stream << "}" << std::endl;
}

void Stats::start_frequent_query() {
    frequent_query_count++;
    start_counting();
}

void Stats::end_frequent_query() {
    finish_counting(frequent_query_time);
}

void Stats::start_k_top_query() {
    k_top_query_count++;
    start_counting();
}

void Stats::end_k_top_query() {
    finish_counting(k_top_query_count);
}

void Stats::start_process_element() {
    process_element_count++;
    start_counting();
}

void Stats::end_process_element() {
    finish_counting(process_element_time);
}
