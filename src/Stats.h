#ifndef _Stats_H_
#define _Stats_H_

#include <chrono>

typedef unsigned long long int counter;

class Stats {

private:
    std::chrono::high_resolution_clock::time_point start_time;

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

};


#endif //_Stats_H_
