#ifndef _SpaceSaving_Algorithm_H_
#define _SpaceSaving_Algorithm_H_

#include "algorithms/GenericAlgorithm.h"
#include "algorithms/space_saving/Types.h"
#include "utils/InputParser.h"
#include <string>


namespace SpaceSaving {


template<class T>
class Algorithm : public GenericAlgorithm<Element, T, FrequencyOrderIterator<Element<T>>> {

private:

    FrequencyOrder<Element<T>> frequency_order;
    unsigned int m;

    // For the new version with fixed threshold
    float threshold;
    TicketGenerator ticket_generator;


    bool insert_element(Element<T>& element) override;

    void update_element(Element<T>& element) override;

public:

    Algorithm(const InputParser& parameters);

    FrequencyOrderIterator<Element<T>> frequency_order_begin() override;

    FrequencyOrderIterator<Element<T>> frequency_order_end() override;

    float get_threshold() const override;

    void print_state() override;
};


}

#include "algorithms/space_saving/Algorithm.ipp"

#endif //_SpaceSaving_Algorithm_H_
