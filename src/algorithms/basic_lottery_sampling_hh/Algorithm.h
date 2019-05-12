#ifndef _BasicLotterySamplingHH_Algorithm_H_
#define _BasicLotterySamplingHH_Algorithm_H_

#include "algorithms/GenericAlgorithm.h"
#include "algorithms/basic_lottery_sampling_hh/Types.h"
#include "utils/InputParser.h"
#include "utils/TicketUtils.h"

namespace BasicLotterySamplingHH {


template<class T>
class Algorithm : public GenericAlgorithm<Element, T, FrequencyOrder<Element<T>>> {

private:

    TicketOrder<Element<T>> ticket_order;
    FrequencyOrder<Element<T>> frequency_order;

    float phi;
    float error;
    float delta;
    float r;

    TicketUtils ticket_generator;

    bool insert_element(Element<T>& element) override;

    void update_element(Element<T>& element) override;

    float get_frequency_threshold(float f) const override;

public:

    Algorithm(const InputParser& parameters);

    FrequencyOrder<Element<T>>& get_frequency_order() override;

    double get_threshold() const override;
};


}

#include "algorithms/basic_lottery_sampling_hh/Algorithm.ipp"

#endif //_BasicLotterySamplingHH_Algorithm_H_
