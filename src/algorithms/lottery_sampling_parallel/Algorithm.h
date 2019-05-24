#ifndef _LotterySamplingParallel_Algorithm_H_
#define _LotterySamplingParallel_Algorithm_H_

#include "algorithms/GenericAlgorithm.h"
#include "algorithms/lottery_sampling_parallel/Types.h"
#include "utils/InputParser.h"
#include "utils/TicketUtils.h"

namespace LotterySamplingParallel {


using namespace std;

template<class T>
class Algorithm : public GenericAlgorithm<Element, T, FrequencyOrder<Element<T>>> {

private:

    FrequencyOrder<Element<T>> frequency_order;
    vector<TicketOrder<ElementInstance<T>>> instances;
    TicketUtils ticket_generator;
    unsigned int m;

    bool insert_element(Element<T>& element) override;

    void update_element(Element<T>& element) override;

public:

    Algorithm(const InputParser& parameters);

    FrequencyOrder<Element<T>>& get_frequency_order() override;
};


}

#include "algorithms/lottery_sampling_parallel/Algorithm.ipp"

#endif //_LotterySamplingParallel_Algorithm_H_
