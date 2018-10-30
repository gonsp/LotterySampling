#ifndef _LotteryCacheSampling_Algorithm_H_
#define _LotteryCacheSampling_Algorithm_H_

#include "algorithms/GenericAlgorithm.h"
#include "algorithms/lottery_cache_sampling/Types.h"
#include "utils/InputParser.h"
#include "utils/TicketUtils.h"


namespace LotteryCacheSampling {


template<class T>
class Algorithm : public GenericAlgorithm<Element, T, FrequencyOrder<Element<T>>> {

private:

    FrequencyOrder<Element<T>> frequency_order;
    CacheOrder<Element<T>> cache_order;

    unsigned int m;
    TicketUtils ticket_generator;
    Ticket mean_ticket;

    bool insert_element(Element<T>& element) override;

    void update_element(Element<T>& element) override;

public:

    Algorithm(const InputParser& parameters);

    FrequencyOrder<Element<T>>& get_frequency_order() override;

    float get_threshold() const override;

    void print_state() override;
};


}

#include "algorithms/lottery_cache_sampling/Algorithm.ipp"

#endif //_LotteryCacheSampling_Algorithm_H_
