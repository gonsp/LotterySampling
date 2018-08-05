#ifndef _SPACESAVING_H_
#define _SPACESAVING_H_

#include "algorithms/GenericAlgorithm.h"
#include "utils/InputParser.h"
#include "Types.h"
#include <string>


namespace SpaceSaving {

template <class T>
class Algorithm : public GenericAlgorithm<T, ElementLocator<T>> {

private:

    typename StreamSummary<T>::type stream_summary;
    unsigned int m;

    void increment_counter(ElementLocator<T>& locator);

    bool insert_element(const T& element_id, ElementLocator<T>& locator) override;

    void update_element(ElementLocator<T>& locator) override;

public:
    Algorithm(const InputParser& parameters);

    void frequent_query(float f, std::ostream& stream) override;

    void k_top_query(int k, std::ostream& stream) override;

    void print_state() override;
};


}

#include "Algorithm.cpp"

#endif //_SPACESAVING_H_
