#ifndef _LotteryCacheSampling_Types_H_
#define _LotteryCacheSampling_Types_H_

#include <data_structures/SortedList.h>
#include "utils/Misc.h"
#include "data_structures/List.h"
#include "data_structures/SortedList.h"

namespace LotteryCacheSampling {


using namespace std;

template<class Element>
using FrequencyOrder = SortedList::SortedList<Element, &Element::frequency_order_locator>;

template<class Element>
using CacheOrder = List::List<Element, &Element::cache_order_locator>;

template<class Element>
using FrequencyOrderLocator = SortedList::Locator<Element>;

template<class Element>
using CacheOrderLocator = List::Locator<Element>;

template<class T>
struct Element {

    T id;
    Ticket ticket;
    unsigned int freq;
    unsigned int initial_estimated_freq;

    FrequencyOrderLocator<Element<T>> frequency_order_locator;
    CacheOrderLocator<Element<T>> cache_order_locator;

    Element(const T& id) {
        this->id = id;
    }
};


}

#endif //_LotteryCacheSampling_Types_H_
