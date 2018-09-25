#ifndef _LotterySpaceSaving_Algorithm_H_
#define _LotterySpaceSaving_Algorithm_H_

#include "algorithms/GenericAlgorithm.h"
#include "algorithms/lottery_space_saving/Types.h"
#include "utils/InputParser.h"
#include "utils/Misc.h"
#include <string>


namespace LotterySpaceSaving {


template<class T>
class Algorithm : public GenericAlgorithm<Element, T, FrequencyOrderIterator<Element<T>>> {

private:

    FrequencyOrder<Element<T>> frequency_order;
    TicketGenerator ticket_generator;
    Ticket mean_ticket;
    float threshold;
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

#include "algorithms/lottery_space_saving/Algorithm.ipp"

#endif //_LotterySpaceSaving_Algorithm_H_
