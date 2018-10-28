#ifndef _LotterySampling_Algorithm_H_
#define _LotterySampling_Algorithm_H_

#include "algorithms/GenericAlgorithm.h"
#include "algorithms/lottery_sampling/Types.h"
#include "utils/InputParser.h"
#include "utils/Misc.h"
#include <string>


namespace LotterySampling {


template<class T>
class Algorithm : public GenericAlgorithm<Element, T, FrequencyOrderIterator<Element<T>>> {

private:

    FrequencyOrder<Element<T>> frequency_order;
    TicketOrder<Element<T>> ticket_order;

    TicketGenerator ticket_generator;
    unsigned int m;

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

#include "algorithms/lottery_sampling/Algorithm.ipp"

#endif //_LotterySampling_Algorithm_H_
