#include "InputParser.h"
#include <iostream>
#include <string>

using namespace std;

InputParser::InputParser(int num_args, char** args) {

    int i = 1;
    while(i < num_args) {
        if(args[i][0] != '-') {
            error();
        }
        if(i < num_args - 1 && args[i+1][0] != '-') {
            parameters.emplace(args[i], args[i+1]);
            ++i;
        } else {
            parameters.emplace(args[i], "");
        }
        ++i;
    }
}

bool InputParser::has_parameter(const string& parameter_name) const {
    return parameters.find(parameter_name) != parameters.end();
}

const string InputParser::get_parameter(const string& parameter_name) const {
    try {
        return parameters.at(parameter_name);
    } catch(std::out_of_range) {
        cerr << "Missing parameter: " << parameter_name << endl;
        error();
        return "";
    }
}

void InputParser::error() const {
    cerr << "Incorrect number of parameters." << endl;
    cerr << "Usage: k-hitting [-a {lottery_sampling, space_saving, lottery_sampling_original, lottery_cache_sampling, lottery_space_saving}] [params]" << endl;
    exit(1);
}
