#ifndef _LotterySamplingV2B_Algorithm_H_
#define _LotterySamplingV2B_Algorithm_H_

#include "algorithms/GenericAlgorithm.h"
#include "algorithms/lottery_sampling_v2_b/Types.h"
#include "utils/TicketUtils.h"


namespace LotterySamplingV2B {


using namespace std;

template<class T>
class Algorithm : public GenericAlgorithm<Element, T, FrequencyOrder<Element<T>>> {

private:

    FrequencyOrder<Element<T>> frequency_order;

    TicketUtils ticket_generator;
    unsigned int m;

    bool insert_element(Element<T>& element) override;

    void update_element(Element<T>& element) override;

public:

    Algorithm(const InputParser& parameters);

    FrequencyOrder<Element<T>>& get_frequency_order() override;

    unordered_map<string, double> get_custom_stats() override;
};


}

#include "algorithms/lottery_sampling_v2_b/Algorithm.ipp"

#endif //_LotterySamplingV2B_Algorithm_H_
