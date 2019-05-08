#include "utils/InputParser.h"
#include "utils/Stats.h"
#include "algorithms/GenericAlgorithm.h"
#include "algorithms/lottery_sampling/Algorithm.h"
#include "algorithms/lottery_sampling_original/Algorithm.h"
#include "algorithms/lottery_sampling_parallel/Algorithm.h"
#include "algorithms/lottery_cache_sampling/Algorithm.h"
#include "algorithms/lottery_space_saving/Algorithm.h"
#include "algorithms/space_saving/Algorithm.h"
#include "algorithms/frequent/Algorithm.h"
#include "algorithms/count_sketch/Algorithm.h"
#include <iostream>
#include <string>

using namespace std;

template<class T>
GenericAlgorithmInterface<T>* create_algorithm_instance(const InputParser& params) {
    if(!params.has_parameter("-a") || params.get_parameter("-a") == "lottery_sampling") {
        return new LotterySampling::Algorithm<T>(params);
    } else if(params.get_parameter("-a") == "space_saving") {
        return new SpaceSaving::Algorithm<T>(params);
    } else if(params.get_parameter("-a") == "lottery_sampling_original") {
        return new LotterySamplingOriginal::Algorithm<T>(params);
    } else if(params.get_parameter("-a") == "lottery_sampling_parallel") {
        return new LotterySamplingParallel::Algorithm<T>(params);
    } else if(params.get_parameter("-a") == "lottery_cache_sampling") {
        return new LotteryCacheSampling::Algorithm<T>(params);
    } else if(params.get_parameter("-a") == "lottery_space_saving") {
        return new LotterySpaceSaving::Algorithm<T>(params);
    } else if(params.get_parameter("-a") == "frequent") {
        return new Frequent::Algorithm<T>(params);
    } else if(params.get_parameter("-a") == "count_sketch") {
        return new CountSketch::Algorithm<T>(params);
    } else if(params.get_parameter("-a") == "count_min") {
        return new CountSketch::Algorithm<T>(params, true);
    } else {
        params.error();
        return nullptr;
    }
}

int main(int num_args, char* args[]) {

    InputParser params(num_args, args);

    GenericAlgorithmInterface<long long int>* algorithm = create_algorithm_instance<long long int>(params);

    Stats stats;
    string s;
    while(cin >> s) {
        if(s == ":q") { // It's a query over the sampled elements
            cin >> s;
            if(s == ":f") { // "More frequent than f" query
                float freq;
                cin >> freq;
                stats.start_frequent_query();
                algorithm->frequent_query(freq, cout);
                stats.end_frequent_query();
            } else if(s == ":k") { // k-top frequent elements query
                int k;
                cin >> k;
                stats.start_k_top_query();
                algorithm->k_top_query(k, cout);
                stats.end_k_top_query();
            }
            cout << ":end" << endl;
        } else if(s == ":s") {
            stats.report(cout, algorithm);
        } else if(s == ":d") {
            algorithm->print_state();
            cout << ":end" << endl;
        } else { // It's a new element in the data stream
            long long int element = stoll(s);
//          string element = s;
            stats.start_process_element();
            algorithm->process_element(element);
            stats.end_process_element();
        }
    }

    delete algorithm;

    return 0;
}

