#ifndef _LotterySamplingV2_Algorithm_H_
#define _LotterySamplingV2_Algorithm_H_

#include "algorithms/GenericAlgorithm.h"
#include "algorithms/lottery_sampling_v2/Types.h"
#include "utils/TicketUtils.h"


namespace LotterySamplingV2 {


using namespace std;

template<class T>
class Algorithm : public GenericAlgorithm<Element, T, FrequencyOrder<Element<T>>> {

private:

    FrequencyOrder<Element<T>> frequency_order;
    ObsFrequencyOrder<Element<T>> obs_frequency_order;

    TicketUtils ticket_generator;
    unsigned int m;
    unsigned int k;
    unsigned int max_k_th_f_obs;

    bool insert_element(Element<T>& element) override;

    void update_element(Element<T>& element) override;

    unsigned int get_k_th_obs_freq(int k);

public:

    Algorithm(const InputParser& parameters);

    FrequencyOrder<Element<T>>& get_frequency_order() override;

    unordered_map<string, double> get_custom_stats() override;
};


}

#include "algorithms/lottery_sampling_v2/Algorithm.ipp"

#endif //_LotterySamplingV2_Algorithm_H_
