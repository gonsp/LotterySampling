#ifndef _LotterySampling_Algorithm_H_
#define _LotterySampling_Algorithm_H_

#include "algorithms/GenericAlgorithm.h"
#include "algorithms/lottery_sampling/Types.h"
#include "utils/InputParser.h"
#include "utils/TicketUtils.h"
#include <string>


namespace LotterySampling {


template<class T>
class Algorithm : public GenericAlgorithm<Element, T, FrequencyOrder<Element<T>>> {

private:

    FrequencyOrder<Element<T>> frequency_order;
    TicketOrder<Element<T>> ticket_order;

    TicketUtils ticket_generator;
    unsigned int m;

    bool insert_element(Element<T>& element) override;

    void update_element(Element<T>& element) override;

public:

    Algorithm(const InputParser& parameters);

    FrequencyOrder<Element<T>>& get_frequency_order() override;

    double get_threshold() const override;

    void print_state() override;
};


}

#include "algorithms/lottery_sampling/Algorithm.ipp"

#endif //_LotterySampling_Algorithm_H_
