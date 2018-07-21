#include <iostream>
#include "InputParser.h"
#include "algorithms/Algorithm.h"
#include "algorithms/lottery_sampling/LotterySampling.h"
#include "algorithms/space_saving/SpaceSaving.h"

int main(int num_args, char* args[]) {

    Algorithm* algorithm;

    InputParser params(num_args, args);
    if(!params.has_parameter("-a") || params.get_parameter("-a") == "lottery_sampling") {
        algorithm = new LotterySampling(params);
    } else if(params.get_parameter("-a") == "space_saving") {
        algorithm = new SpaceSaving(params);
    }

    return 0;
}

