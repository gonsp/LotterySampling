#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_

#include <ostream>
#include "Stats.h"

class Algorithm {

public:
    virtual void frequent_query(std::ostream& stream) = 0;

    virtual void k_top_query(std::ostream& stream) = 0;

    virtual void process_element(std::string& element_id) = 0;

    virtual void print_state() = 0; // For debugging purposes
};

#endif //_ALGORITHM_H_
