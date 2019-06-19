#ifndef _LotterySampling_Algorithm_H_
#define _LotterySampling_Algorithm_H_

#include "algorithms/GenericAlgorithm.h"
#include "algorithms/lottery_sampling/Types.h"
#include "utils/TicketUtils.h"


namespace LotterySampling {


using namespace std;

template<class T>
class Algorithm : public GenericAlgorithm<Element, T, FrequencyOrder<Element<T>>> {

private:

    FrequencyOrder<Element<T>> frequency_order;
    TicketOrder<Element<T>> ticket_order;

    TicketUtils ticket_generator;
    unsigned int m;
    double phi;
    int leading_ones;

    void process_element(const T& element_id) override;

    bool insert_element(Element<T>& element) override;

    void update_element(Element<T>& element) override;

public:

    Algorithm(const InputParser& parameters);

    FrequencyOrder<Element<T>>& get_frequency_order() override;

    double get_frequency_threshold(double f) const override;

    unordered_map<string, double> get_custom_stats() const override;
};


}

#include "algorithms/lottery_sampling/Algorithm.ipp"

#endif //_LotterySampling_Algorithm_H_
