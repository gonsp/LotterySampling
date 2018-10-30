#ifndef _LotterySamplingOriginal_Algorithm_H_
#define _LotterySamplingOriginal_Algorithm_H_

#include "algorithms/GenericAlgorithm.h"
#include "algorithms/lottery_sampling_original/Types.h"
#include "utils/InputParser.h"
#include "utils/TicketUtils.h"

namespace LotterySamplingOriginal {


template<class T>
class Algorithm : public GenericAlgorithm<Element, T, FrequencyOrderIterator<Element<T>>> {

private:

    TicketOrder<Element<T>> level_1;
    TicketOrder<Element<T>> level_2;

    FrequencyOrder<Element<T>> frequency_order;

    unsigned int m;
    bool multilevel;

    TicketUtils ticket_generator;

    bool insert_element(Element<T>& element) override;

    void update_element(Element<T>& element) override;

    void insert_level_1(Element<T>& element);

    void insert_level_2(Element<T>& element);

public:

    Algorithm(const InputParser& parameters);

    FrequencyOrderIterator<Element<T>> frequency_order_begin() override;

    FrequencyOrderIterator<Element<T>> frequency_order_end() override;

    float get_threshold() const override;

    void print_state() override;

    void print_level(TicketOrder<Element<T>>& level);
};


}

#include "algorithms/lottery_sampling_original/Algorithm.ipp"

#endif //_LotterySamplingOriginal_Algorithm_H_
