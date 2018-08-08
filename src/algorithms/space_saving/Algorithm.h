#ifndef _SPACESAVING_H_
#define _SPACESAVING_H_

#include "algorithms/GenericAlgorithm.h"
#include "utils/InputParser.h"
#include "DataStructures.h"
#include <string>


namespace SpaceSaving {


template<class T>
class Algorithm : public GenericAlgorithm<Element, T> {

private:

    typename Element<T>::StreamSummary stream_summary;
    unsigned int m;

    void increment_counter(Element<T>& element);

    bool insert_element(Element<T>& element) override;

    void update_element(Element<T>& element) override;

public:
    Algorithm(const InputParser& parameters);

    void frequent_query(float f, std::ostream& stream) override;

    void k_top_query(int k, std::ostream& stream) override;

    void print_state() const override;
};


}

#include "Algorithm.ipp"

#endif //_SPACESAVING_H_
