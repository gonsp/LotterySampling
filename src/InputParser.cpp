#include "InputParser.h"
#include <iostream>
#include <string>

using namespace std;

InputParser::InputParser(int num_args, char** args) {

    if((num_args-1) % 2 != 0) {
        error();
    }

    for(int i = 1; i < num_args; i += 2) {
        if(args[i][0] != '-') {
            error();
        }
        parameters.insert(pair<string, string>(args[i], args[i+1]));
    }
}

bool InputParser::has_parameter(const string& parameter_name) const {
    return parameters.find(parameter_name) != parameters.end();
}

const string InputParser::get_parameter(const string& parameter_name) const {
    return parameters.at(parameter_name);
}

void InputParser::error() {
    cerr << "Incorrect number of parameters." << endl;
    cerr << "Usage: k-hitting [-a {lottery_sampling, space_saving}] [params]" << endl;
    exit(1);
}
