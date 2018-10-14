#ifndef _CountSketch_Algorithm_H_
#define _CountSketch_Algorithm_H_

#include "algorithms/GenericAlgorithm.h"
#include "algorithms/count_sketch/Types.h"
#include "utils/InputParser.h"


namespace CountSketch {


template<class T>
class Algorithm : public GenericAlgorithm<Element, T, FrequencyOrderIterator<Element<T>>> {

private:

    FrequencyOrder<Element<T>> frequency_order;
    unsigned int m;
    unsigned int h;
    Counters counters;
    Hasher<T> hasher;

    bool insert_element(Element<T>& element) override;

    void update_element(Element<T>& element) override;

    int update_count(Element<T>& element);

public:

    Algorithm(const InputParser& parameters);

    FrequencyOrderIterator<Element<T>> frequency_order_begin() override;

    FrequencyOrderIterator<Element<T>> frequency_order_end() override;

    float get_threshold() const override;

    void print_state() override;
};


}

#include "algorithms/count_sketch/Algorithm.ipp"

#endif //_CountSketch_Algorithm_H_
