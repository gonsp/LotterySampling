#ifndef _CountSketch_Algorithm_H_
#define _CountSketch_Algorithm_H_

#include "algorithms/GenericAlgorithm.h"
#include "algorithms/count_sketch/Types.h"
#include "utils/InputParser.h"


namespace CountSketch {


template<class T>
class Algorithm : public GenericAlgorithm<Element, T, FrequencyOrder<Element<T>>> {

private:

    FrequencyOrder<Element<T>> frequency_order;
    unsigned int m;
    unsigned int h;
    unsigned int q;
    Counters counters;
    Hasher<T> element_hasher;
    Hasher<size_t> hash_hasher;
    bool count_min;

    bool insert_element(Element<T>& element) override;

    void update_element(Element<T>& element) override;

    int update_count(Element<T>& element);

public:

    Algorithm(const InputParser& parameters) : Algorithm(parameters, false) {};

    Algorithm(const InputParser& parameters, bool count_min);

    FrequencyOrder<Element<T>>& get_frequency_order() override;

    void print_state() override;
};


}

#include "algorithms/count_sketch/Algorithm.ipp"

#endif //_CountSketch_Algorithm_H_
