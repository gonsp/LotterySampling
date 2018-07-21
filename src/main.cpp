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

    string s;
    while(cin >> s) {
        if(s == "q") { // It's a query over the sampled elements
            cin >> s;
            if(s == "f") { // "More frequent than f" query
                float f;
                cin >> f;
                algorithm->frequent_query(cout);
            } else if(s == "k") { // k-top frequent elements query
                int k;
                cin >> k;
                algorithm->k_top_query(cout);
            }
        } else { // It's a new element in the data stream
            algorithm->process_element(s);
        }
    }

    return 0;
}

