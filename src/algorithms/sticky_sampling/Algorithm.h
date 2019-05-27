#ifndef _StickySampling_Algorithm_H_
#define _StickySampling_Algorithm_H_

#include "algorithms/GenericAlgorithm.h"
#include "algorithms/sticky_sampling/Types.h"


namespace StickySampling {


using namespace std;

template<class T>
class Algorithm : public GenericAlgorithm<Element, T, FrequencyOrder<Element<T>>> {

private:

    FrequencyOrder<Element<T>> frequency_order;

    double phi;
    double error;
    double delta;
    unsigned int r;
    unsigned int t;
    unsigned int next_resampling;

    TicketUtils ticket_generator;

    void process_element(const T& element_id) override;

    bool insert_element(Element<T>& element) override;

    void update_element(Element<T>& element) override;

    bool toss_coin(double p);

public:

    Algorithm(const InputParser& parameters);

    FrequencyOrder<Element<T>>& get_frequency_order() override;

    double get_frequency_threshold(double f) const override;
};


}

#include "algorithms/sticky_sampling/Algorithm.ipp"

#endif //_StickySampling_Algorithm_H_
