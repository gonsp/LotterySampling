#ifndef _SPACESAVING_H_
#define _SPACESAVING_H_

#include "algorithms/GenericAlgorithm.h"
#include "utils/InputParser.h"
#include "Types.h"
#include <string>


namespace SpaceSaving {


template<class T>
class Algorithm : public GenericAlgorithm<Element, T> {

private:

    FrequencyOrder<Element<T>> frequency_order;
    unsigned int m;

    bool insert_element(Element<T>& element) override;

    void update_element(Element<T>& element) override;

public:

    Algorithm(const InputParser& parameters);

    void frequent_query(float f, std::ostream& stream) override;

    void k_top_query(int k, std::ostream& stream) override;

    void print_state() override;
};


}

#include "Algorithm.ipp"

#endif //_SPACESAVING_H_
