#ifndef _Stats_H_
#define _Stats_H_

#include <chrono>
#include <ostream>
#include <algorithms/GenericAlgorithm.h>


class Stats {

private:

    typedef long long int counter;
    typedef std::chrono::high_resolution_clock::time_point time_point;

    time_point initial_time;
    time_point start_time;

    counter get_interval(time_point from);

    void start_counting();

    void finish_counting(counter& counter);

public:

    counter frequent_query_time;
    counter frequent_query_count;
    counter k_top_query_time;
    counter k_top_query_count;
    counter process_element_time;
    counter process_element_count;

    Stats();

    void start_frequent_query();

    void end_frequent_query();

    void start_k_top_query();

    void end_k_top_query();

    void start_process_element();

    void end_process_element();

    template<class T>
    void report(std::ostream& stream, GenericAlgorithmInterface<T>* algorithm);
};

#include "Stats.ipp"


#endif //_Stats_H_
