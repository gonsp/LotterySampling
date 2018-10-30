#ifndef _LotterySpaceSaving_Algorithm_H_
#define _LotterySpaceSaving_Algorithm_H_

#include "algorithms/GenericAlgorithm.h"
#include "algorithms/lottery_space_saving/Types.h"
#include "utils/InputParser.h"
#include "utils/TicketUtils.h"
#include <string>


namespace LotterySpaceSaving {


template<class T>
class Algorithm : public GenericAlgorithm<Element, T, FrequencyOrder<Element<T>>> {

private:

    FrequencyOrder<Element<T>> frequency_order;
    TicketUtils ticket_generator;
    Ticket mean_ticket;
    unsigned int m;

    bool insert_element(Element<T>& element) override;

    void update_element(Element<T>& element) override;

public:

    Algorithm(const InputParser& parameters);

    FrequencyOrder<Element<T>>& get_frequency_order() override;

    float get_threshold() const override;

    void print_state() override;
};


}

#include "algorithms/lottery_space_saving/Algorithm.ipp"

#endif //_LotterySpaceSaving_Algorithm_H_
