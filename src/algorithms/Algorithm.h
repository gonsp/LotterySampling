#ifndef _ALGORITHM_H_
#define _ALGORITHM_H_

#include <ostream>

class Algorithm {

public:
    void frequent_query(std::ostream& stream);

    void k_top_query(std::ostream& stream);

    void process_element(std::string element);

};

#endif //_ALGORITHM_H_
