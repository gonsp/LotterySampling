#ifndef _LotterySamplingV2C_Types_H_
#define _LotterySamplingV2C_Types_H_

#include "data_structures/SortedList.h"
#include "data_structures/SortedVector.h"
#include "utils/TicketUtils.h"


namespace LotterySamplingV2C {


template<class Element>
using FrequencyOrder = SortedList::SortedList<Element, &Element::frequency_order_locator>;

template<class Element>
using ObsFrequencyOrder = SortedList::SortedList<Element, &Element::obs_frequency_order_locator>;

template<class Element>
using FrequencyOrderLocator = SortedList::Locator<Element>;

template<class Element>
using ObsFrequencyOrderLocator = SortedList::Locator<Element>;


template<class T>
struct Element {

    T id;

    Element(const T& id) {
        this->id = id;
    }

    unsigned int get_freq() const {
        return frequency_order_locator.bucket_iterator->key;
    }

    unsigned int get_obs_freq() const {
        return obs_frequency_order_locator.bucket_iterator->key;
    }

    FrequencyOrderLocator<Element<T>> frequency_order_locator;
    ObsFrequencyOrderLocator<Element<T>> obs_frequency_order_locator;
};


}

#endif //_LotterySamplingV2C_Types_H_
