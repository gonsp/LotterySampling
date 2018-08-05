#ifndef _Stats_H_
#define _Stats_H_

#include <chrono>
#include <ostream>
#include <algorithms/GenericAlgorithm.h>
#include <malloc/malloc.h>

typedef long long int counter;

class Stats {

private:

    typedef std::chrono::high_resolution_clock::time_point time_point;
    time_point initial_time;
    time_point start_time;

    counter get_interval(time_point from);

public:

    counter frequent_query_time;
    counter frequent_query_count;
    counter k_top_query_time;
    counter k_top_query_count;
    counter process_element_time;
    counter process_element_count;

    Stats();

    void start_counting(counter& counter);

    void finish_counting(counter& counter);

    template <class T>
    void report(std::ostream& stream, GenericAlgorithmInterface<T>* algorithm) {
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
};


#endif //_Stats_H_
