#ifndef _BasicLotterySamplingTopK_Algorithm_H_
#define _BasicLotterySamplingTopK_Algorithm_H_

#include "algorithms/GenericAlgorithm.h"
#include "algorithms/basic_lottery_sampling_top_k/Types.h"
#include "utils/TicketUtils.h"


namespace BasicLotterySamplingTopK {


using namespace std;

template<class T>
class Algorithm : public GenericAlgorithm<Element, T, FrequencyOrder<Element<T>>> {

private:

    mutable FrequencyOrder<Element<T>> frequency_order;
    mutable TicketOrder<Element<T>> ticket_order;

    unsigned int k;
    double delta;
    double error;
    double r;

    TicketUtils ticket_generator;

    int k_th_freq_obs;
    int prev_count;

    bool insert_element(Element<T>& element) override;

    void update_element(Element<T>& element) override;

    double get_threshold() const;

public:

    Algorithm(const InputParser& parameters);

    FrequencyOrder<Element<T>>& get_frequency_order() override;

    unordered_map<string, double> get_custom_stats() override;

    QueryResults<T> top_k_query(int k) override;
};


}

#include "algorithms/basic_lottery_sampling_top_k/Algorithm.ipp"

#endif //_BasicLotterySamplingTopK_Algorithm_H_
