#ifndef _BasicLotterySampling_Algorithm_H_
#define _BasicLotterySampling_Algorithm_H_

#include "algorithms/GenericAlgorithm.h"
#include "algorithms/basic_lottery_sampling/Types.h"
#include "utils/TicketUtils.h"


namespace BasicLotterySampling {


using namespace std;

template<class T>
class Algorithm : public GenericAlgorithm<Element, T, FrequencyOrder<Element<T>>> {

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

    FrequencyOrder<Element<T>>& get_frequency_order() override;

    unordered_map<string, double> get_custom_stats() override;
};


}

#include "algorithms/basic_lottery_sampling/Algorithm.ipp"

#endif //_BasicLotterySampling_Algorithm_H_
