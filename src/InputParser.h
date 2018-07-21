#ifndef _InputParser_H_
#define _InputParser_H_

#include <map>
#include <string>

class InputParser {

private:
    std::map<std::string, std::string> parameters;

    void error();

public:
    InputParser(int num_args, char* args[]);

    bool has_parameter(const std::string& parameter_name) const;

    const std::string get_parameter(const std::string& parameter_name) const;
};

#endif //_InputParser_H_
