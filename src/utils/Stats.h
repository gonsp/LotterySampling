#ifndef _Stats_H_
#define _Stats_H_

#include <chrono>
#include <ostream>
#include <algorithms/GenericAlgorithm.h>

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

    void report(std::ostream& stream, GenericAlgorithmInterface* algorithm);
};


#endif //_Stats_H_
