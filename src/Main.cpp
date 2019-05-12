#include "utils/InputParser.h"
#include "utils/Stats.h"
#include "algorithms/GenericAlgorithm.h"
#include "algorithms/lottery_sampling/Algorithm.h"
#include "algorithms/basic_lottery_sampling/Algorithm.h"
#include "algorithms/basic_lottery_sampling_hh/Algorithm.h"
#include "algorithms/lottery_sampling_parallel/Algorithm.h"
#include "algorithms/space_saving/Algorithm.h"
#include "algorithms/frequent/Algorithm.h"
#include "algorithms/count_sketch/Algorithm.h"
#include <iostream>
#include <string>

using namespace std;

template<class T>
GenericAlgorithmInterface<T>* create_algorithm_instance(const InputParser& params) {
    if(!params.has_parameter("-a") || params.get_parameter("-a") == "LotterySampling") {
        return new LotterySampling::Algorithm<T>(params);
    } else if(params.get_parameter("-a") == "SpaceSaving") {
        return new SpaceSaving::Algorithm<T>(params);
    } else if(params.get_parameter("-a") == "BasicLotterySampling") {
        return new BasicLotterySampling::Algorithm<T>(params);
    } else if(params.get_parameter("-a") == "BasicLotterySamplingHH") {
        return new BasicLotterySamplingHH::Algorithm<T>(params);
    } else if(params.get_parameter("-a") == "LotterySamplingParallel") {
        return new LotterySamplingParallel::Algorithm<T>(params);
    } else if(params.get_parameter("-a") == "Frequent") {
        return new Frequent::Algorithm<T>(params);
    } else if(params.get_parameter("-a") == "CountSketch") {
        return new CountSketch::Algorithm<T>(params);
    } else if(params.get_parameter("-a") == "CountMin") {
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
            if(s == ":f") { // Heavy hitters query
                float freq;
                cin >> freq;
                stats.start_frequent_query();
                algorithm->frequent_query(freq, cout);
                stats.end_frequent_query();
            } else if(s == ":k") { // k-top frequent elements query
                int k;
                cin >> k;
                stats.start_top_k_query();
                algorithm->top_k_query(k, cout);
                stats.end_top_k_query();
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

