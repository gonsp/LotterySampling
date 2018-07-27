#ifndef _InputParser_H_
#define _InputParser_H_

#include <map>
#include <string>

class InputParser {

private:
    std::map<std::string, std::string> parameters;

public:
    InputParser(int num_args, char* args[]);

    bool has_parameter(const std::string& parameter_name) const;

    const std::string get_parameter(const std::string& parameter_name) const;

    void error() const;
};

#endif //_InputParser_H_
