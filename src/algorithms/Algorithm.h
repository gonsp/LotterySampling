#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_

#include <ostream>
#include "Stats.h"

class Algorithm {

protected:

    int N = 0;

    virtual void process_element(std::string& element_id) = 0;

public:
    virtual void frequent_query(float f, std::ostream& stream) = 0;

    virtual void k_top_query(int k, std::ostream& stream) = 0;

    void register_element(std::string& element_id) {
        N++;
        process_element(element_id);
    }

    virtual void print_state() = 0; // For debugging purposes
};


#endif //_ALGORITHM_H_
