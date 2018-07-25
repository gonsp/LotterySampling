#include "InputParser.h"
#include "algorithms/Algorithm.h"
#include "algorithms/lottery_sampling/LotterySampling.h"
#include "algorithms/space_saving/SpaceSaving.h"
#include <iostream>
#include <string>

using namespace std;

int main(int num_args, char* args[]) {

    InputParser params(num_args, args);

    Algorithm* algorithm;
    if(!params.has_parameter("-a") || params.get_parameter("-a") == "lottery_sampling") {
        algorithm = new LotterySampling(params);
    } else if(params.get_parameter("-a") == "space_saving") {
        algorithm = new SpaceSaving(params);
    }

    Stats stats;
    string s;
    while(cin >> s) {
        if(s == ":q") { // It's a query over the sampled elements
            cin >> s;
            if(s == ":f") { // "More frequent than f" query
                float f;
                cin >> f;
                stats.start_counting(stats.frequent_query_count);
                algorithm->frequent_query(f, cout);
                stats.finish_counting(stats.frequent_query_time);
            } else if(s == ":k") { // k-top frequent elements query
                int k;
                cin >> k;
                stats.start_counting(stats.k_top_query_count);
                algorithm->k_top_query(k, cout);
                stats.finish_counting(stats.k_top_query_time);
            }
        } else if(s == ":s") {
            stats.report(cout);
        } else if(s == ":d") {
            algorithm->print_state();
        } else { // It's a new element in the data stream
            stats.start_counting(stats.process_element_count);
            algorithm->register_element(s);
            stats.start_counting(stats.process_element_time);
        }
    }

    return 0;
}

