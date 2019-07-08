#ifndef _BasicLotterySamplingHH_Algorithm_H_
#define _BasicLotterySamplingHH_Algorithm_H_

#include "algorithms/GenericAlgorithm.h"
#include "algorithms/basic_lottery_sampling_hh/Types.h"
#include "utils/TicketUtils.h"


namespace BasicLotterySamplingHH {


using namespace std;

template<class T>
class Algorithm : public GenericAlgorithm<Element, T, FrequencyOrder<Element<T>>> {

private:

    TicketOrder<Element<T>> ticket_order;
    FrequencyOrder<Element<T>> frequency_order;

    double phi;
    double error;
    double delta;
    double r;

    TicketUtils ticket_generator;

    bool insert_element(Element<T>& element) override;

    void update_element(Element<T>& element) override;

    double get_threshold() const;

public:

    Algorithm(const InputParser& parameters);

    FrequencyOrder<Element<T>>& get_frequency_order() override;

    double get_frequency_threshold(double f) const override;

    unordered_map<string, double> get_custom_stats() override;
};


}

#include "algorithms/basic_lottery_sampling_hh/Algorithm.ipp"

#endif //_BasicLotterySamplingHH_Algorithm_H_
