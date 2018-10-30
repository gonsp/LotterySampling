#ifndef _LotteryCacheSampling_Types_H_
#define _LotteryCacheSampling_Types_H_

#include "utils/TicketUtils.h"
#include "data_structures/SortedList.h"
#include <data_structures/SortedVector.h>
#include "data_structures/List.h"

namespace LotteryCacheSampling {


using namespace std;

template<class Element>
using FrequencyOrder = SortedVector::SortedVector<Element, &Element::frequency_order_locator>;

template<class Element>
using CacheOrder = List::List<Element, &Element::cache_order_locator>;

template<class Element>
using FrequencyOrderLocator = SortedVector::Locator;

template<class Element>
using CacheOrderLocator = List::Locator<Element>;

template<class Element>
using FrequencyOrderIterator = SortedVector::Iterator<Element>;

template<class T>
struct Element {

    T id;
    Ticket ticket;
    unsigned int over_estimation;

    FrequencyOrderLocator<Element<T>> frequency_order_locator;
    CacheOrderLocator<Element<T>> cache_order_locator;

    Element(const T& id) {
        this->id = id;
    }

    unsigned int get_count() const {
        return frequency_order_locator.bucket_iterator->key;
    }
};


}

#endif //_LotteryCacheSampling_Types_H_
