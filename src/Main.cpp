#include "utils/InputParser.h"
#include "utils/Stats.h"
#include "algorithms/GenericAlgorithm.h"
#include "algorithms/lottery_sampling/Algorithm.h"
#include "algorithms/lottery_sampling_hh/Algorithm.h"
#include "algorithms/basic_lottery_sampling/Algorithm.h"
#include "algorithms/basic_lottery_sampling_hh/Algorithm.h"
#include "algorithms/basic_lottery_sampling_top_k/Algorithm.h"
#include "algorithms/lottery_sampling_parallel/Algorithm.h"
#include "algorithms/space_saving/Algorithm.h"
#include "algorithms/frequent/Algorithm.h"
#include "algorithms/lossy_counting/Algorithm.h"
#include "algorithms/sticky_sampling/Algorithm.h"
#include "algorithms/count_sketch/Algorithm.h"
#include <iostream>
#include <string>

using namespace std;

template<class T>
GenericAlgorithmInterface<T>* create_algorithm_instance(const InputParser& params) {
    if(!params.has_parameter("-a") || params.get_parameter("-a") == "LotterySampling") {
        return new LotterySampling::Algorithm<T>(params);
    } else if(params.get_parameter("-a") == "LotterySamplingHH") {
        return new LotterySamplingHH::Algorithm<T>(params);
    } else if(params.get_parameter("-a") == "BasicLotterySampling") {
        return new BasicLotterySampling::Algorithm<T>(params);
    } else if(params.get_parameter("-a") == "BasicLotterySamplingHH") {
        return new BasicLotterySamplingHH::Algorithm<T>(params);
    } else if(params.get_parameter("-a") == "BasicLotterySamplingTopK") {
        return new BasicLotterySamplingTopK::Algorithm<T>(params);
    } else if(params.get_parameter("-a") == "SpaceSaving") {
        return new SpaceSaving::Algorithm<T>(params);
    } else if(params.get_parameter("-a") == "LotterySamplingParallel") {
        return new LotterySamplingParallel::Algorithm<T>(params);
    } else if(params.get_parameter("-a") == "Frequent") {
        return new Frequent::Algorithm<T>(params);
    } else if(params.get_parameter("-a") == "LossyCounting") {
        return new LossyCounting::Algorithm<T>(params);
    } else if(params.get_parameter("-a") == "StickySampling") {
        return new StickySampling::Algorithm<T>(params);
    } else if(params.get_parameter("-a") == "CountSketch") {
        return new CountSketch::Algorithm<T>(params);
    } else if(params.get_parameter("-a") == "CountMin") {
        return new CountSketch::Algorithm<T>(params, true);
    } else {
        params.error();
        return nullptr;
    }
}

template<class T>
void print_results(QueryResults<T> results) {
    for(auto it = results.begin(); it != results.end(); ++it) {
        cout << it->first << " " << it->second << endl;
    }
}

int main(int num_args, char* args[]) {

    InputParser params(num_args, args);

    typedef long long int T;
    GenericAlgorithmInterface<T>* algorithm = create_algorithm_instance<T>(params);

    Stats stats;
    string s;
    while(cin >> s) {
        if(s == ":q") { // It's a query over the sampled elements
            cin >> s;
            if(s == ":f") { // Heavy hitters query
                double freq;
                cin >> freq;
                stats.start_frequent_query();
                print_results(algorithm->frequent_query(freq, cout));
                stats.end_frequent_query();
            } else if(s == ":k") { // k-top frequent elements query
                int k;
                cin >> k;
                stats.start_top_k_query();
                print_results(algorithm->top_k_query(k, cout));
                stats.end_top_k_query();
            }
            cout << ":end" << endl;
        } else if(s == ":s") {
            stats.report(cout, algorithm);
        } else { // It's a new element in the data stream
            T element = stoll(s);
//          T element = s;
            stats.start_process_element();
            algorithm->process_element(element);
            stats.end_process_element();
        }
    }

    delete algorithm;

    return 0;
}
