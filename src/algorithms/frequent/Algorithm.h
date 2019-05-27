#ifndef _Frequent_Algorithm_H_
#define _Frequent_Algorithm_H_

#include "algorithms/GenericAlgorithm.h"
#include "algorithms/frequent/Types.h"


namespace Frequent {


template<class T>
class Algorithm : public GenericAlgorithm<Element, T, FrequencyOrder<Element<T>>> {

private:

    FrequencyOrder<Element<T>> frequency_order;
    unsigned int m;
    unsigned int offset;

    bool insert_element(Element<T>& element) override;

    void update_element(Element<T>& element) override;

public:

    Algorithm(const InputParser& parameters);

    FrequencyOrder<Element<T>>& get_frequency_order() override;
};


}

#include "algorithms/frequent/Algorithm.ipp"

#endif //_Frequent_Algorithm_H_
