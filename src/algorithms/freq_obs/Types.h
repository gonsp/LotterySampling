#ifndef _FreqObs_Types_H_
#define _FreqObs_Types_H_

#include "data_structures/SortedVector.h"


namespace FreqObs {


template<class Element>
using FrequencyOrder = SortedVector::SortedVector<Element, &Element::frequency_order_locator>;

template<class Element>
using FrequencyOrderLocator = SortedVector::Locator;


template<class T>
struct Element {

    T id;

    Element(const T& id) {
        this->id = id;
    }

    unsigned int get_freq() const {
        return frequency_order_locator.bucket_iterator->key;
    }

    FrequencyOrderLocator<Element<T>> frequency_order_locator;
};


}

#endif //_FreqObs_Types_H_
