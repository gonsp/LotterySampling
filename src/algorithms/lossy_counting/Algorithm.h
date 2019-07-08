#ifndef _LossyCounting_Algorithm_H_
#define _LossyCounting_Algorithm_H_

#include "algorithms/GenericAlgorithm.h"
#include "algorithms/lossy_counting/Types.h"


namespace LossyCounting {


template<class T>
class Algorithm : public GenericAlgorithm<Element, T, FrequencyOrder<Element<T>>> {

private:

    FrequencyOrder<Element<T>> frequency_order;

    double error;

    bool insert_element(Element<T>& element) override;

    void update_element(Element<T>& element) override;

public:

    Algorithm(const InputParser& parameters);

    FrequencyOrder<Element<T>>& get_frequency_order() override;
};


}

#include "algorithms/lossy_counting/Algorithm.ipp"

#endif //_LossyCounting_Algorithm_H_
