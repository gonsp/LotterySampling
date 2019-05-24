#include <algorithms/GenericAlgorithm.h>
#include "Stats.h"


Stats::Stats() {
    frequent_query_time = 0;
    frequent_query_count = 0;
    top_k_query_time = 0;
    top_k_query_count = 0;
    process_element_time = 0;
    process_element_count = 0;

    initial_time = std::chrono::high_resolution_clock::now();
}

Stats::counter Stats::get_interval(time_point from) {
    time_point to = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::nanoseconds>(to - from).count();
}

void Stats::start_counting() {
    start_time = std::chrono::high_resolution_clock::now();
}

void Stats::finish_counting(counter& counter) {
    counter += get_interval(start_time);
}

#if __APPLE__
#include <malloc/malloc.h>
#endif

template<class T>
void Stats::report(std::ostream& stream, GenericAlgorithmInterface<T>* algorithm) {
    std::unordered_map<std::string, double> custom_stats = algorithm->get_custom_stats();
    // Python string format to build a dictionary
    stream << "{";
    for(auto it = custom_stats.begin(); it != custom_stats.end(); ++it) {
        stream << "'" << it->first << "': " << it->second << ",";
    }
    stream << "'sample_size': " << algorithm->sample_size() << ",";
    stream << "'total_exec_time': " << process_element_time << ",";
    stream << "'average_exec_time': " << double(process_element_time) / process_element_count << ",";
    stream << "'N': " << process_element_count << ",";
    stream << "'frequent_query_time': " << frequent_query_time << ",";
    stream << "'frequent_query_count': " << frequent_query_count << ",";
    stream << "'top_k_query_time': " << top_k_query_time << ",";
    stream << "'top_k_query_count': " << top_k_query_count << ",";
#if __APPLE__
    stream << "'memory_usage': " << mstats().bytes_used;
#else
    stream << "'memory_usage': " << -1;
#endif
    stream << "}" << std::endl;
}

void Stats::start_frequent_query() {
    frequent_query_count++;
    start_counting();
}

void Stats::end_frequent_query() {
    finish_counting(frequent_query_time);
}

void Stats::start_top_k_query() {
    top_k_query_count++;
    start_counting();
}

void Stats::end_top_k_query() {
    finish_counting(top_k_query_count);
}

void Stats::start_process_element() {
    process_element_count++;
    start_counting();
}

void Stats::end_process_element() {
    finish_counting(process_element_time);
}
