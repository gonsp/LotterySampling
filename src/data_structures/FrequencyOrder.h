#ifndef _Frequency_LotterySampling_H_
#define _Frequency_LotterySampling_H_

#include <set>
#include "utils/Misc.h"


template<class Element, class FrequencyComparator = PointerComparator>
class FrequencyOrder : private std::multiset<Element*, FrequencyComparator> {
// - Logarithmic insertions of arbitrary values of frequency.
// - Constant time deletion of arbitrary elements.
// - Constant time frequency increment (1 unit).
// - Logarithmic time frequency arbitrary modification.
// - Linear time ordered traversal through iterators.

public:
    void insert_element(Element* element);

    void remove_element(Element* element);

    void increment_frequency(Element* element);

    using typename std::multiset<Element*, FrequencyComparator>::iterator;

    using std::multiset<Element*, FrequencyComparator>::size;

    using std::multiset<Element*, FrequencyComparator>::begin;

    using std::multiset<Element*, FrequencyComparator>::end;

    using std::multiset<Element*, FrequencyComparator>::rbegin;

    using std::multiset<Element*, FrequencyComparator>::rend;
};


#include "FrequencyOrder.ipp"

#endif //_Frequency_LotterySampling_H_