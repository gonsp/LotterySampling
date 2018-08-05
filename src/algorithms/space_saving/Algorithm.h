#ifndef _SPACESAVING_H_
#define _SPACESAVING_H_

#include "algorithms/GenericAlgorithm.h"
#include "utils/InputParser.h"
#include "Types.h"
#include <string>


namespace SpaceSaving {

template <class T>
class Algorithm : public GenericAlgorithm<T, typename ElementLocator<T>::type> {

private:

    typedef typename ElementLocator<T>::type Locator;

    typename StreamSummary<T>::type stream_summary;
    unsigned int m;

    void increment_counter(Locator& locator);

    bool insert_element(const T& element_id, Locator& locator) override;

    void update_element(Locator& locator) override;

public:
    Algorithm(const InputParser& parameters);

    void frequent_query(float f, std::ostream& stream) override;

    void k_top_query(int k, std::ostream& stream) override;

    void print_state() override;
};


}

#include "Algorithm.ipp"

#endif //_SPACESAVING_H_
