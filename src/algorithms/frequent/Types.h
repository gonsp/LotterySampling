#ifndef _Frequent_Types_H_
#define _Frequent_Types_H_

#include "data_structures/SortedList.h"
#include "data_structures/SortedVector.h"

namespace Frequent {


template<class Element>
using FrequencyOrder = SortedVector::SortedVector<Element, &Element::frequency_order_locator>;

template<class Element>
using FrequencyOrderLocator = SortedVector::Locator;


template<class T>
struct Element {

    T id;

    FrequencyOrderLocator<Element<T>> frequency_order_locator;

    Element(const T& id) {
        this->id = id;
    }

    unsigned int get_count() const {
        return frequency_order_locator.bucket_iterator->key;
    }
};


}

#endif //_Frequent_Types_H_
