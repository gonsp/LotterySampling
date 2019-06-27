#ifndef _FreqObs_Algorithm_H_
#define _FreqObs_Algorithm_H_

#include "algorithms/GenericAlgorithm.h"
#include "algorithms/freq_obs/Types.h"


namespace FreqObs {


using namespace std;

template<class T>
class Algorithm : public GenericAlgorithm<Element, T, FrequencyOrder<Element<T>>> {

private:

    FrequencyOrder<Element<T>> frequency_order;

    unsigned int m;

    bool insert_element(Element<T>& element) override;

    void update_element(Element<T>& element) override;

public:

    Algorithm(const InputParser& parameters);

    FrequencyOrder<Element<T>>& get_frequency_order() override;
};


}

#include "algorithms/freq_obs/Algorithm.ipp"

#endif //_FreqObs_Algorithm_H_
