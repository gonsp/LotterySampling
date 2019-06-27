#ifndef _LotterySamplingV2_Types_H_
#define _LotterySamplingV2_Types_H_

#include "data_structures/SortedList.h"
#include "data_structures/SortedTree.h"
#include "utils/TicketUtils.h"


namespace LotterySamplingV2 {


template<class Element>
using FrequencyOrder = SortedTree::SortedTree<Element, &Element::compare_freq, &Element::frequency_order_locator>;

template<class Element>
using ObsFrequencyOrder = SortedList::SortedList<Element, &Element::obs_frequency_order_locator>;

template<class Element>
using FrequencyOrderLocator = SortedTree::Locator<Element, &Element::compare_freq>;

template<class Element>
using ObsFrequencyOrderLocator = SortedList::Locator<Element>;


template<class T>
struct Element {

    T id;
    unsigned int freq;

    Element(const T& id) {
        this->id = id;
    }

    unsigned int get_freq() const {
        return freq;
    }

    unsigned int get_obs_freq() const {
        return obs_frequency_order_locator.bucket_iterator->key;
    }

    bool compare_freq(const Element<T>& element) const {
        return this->freq < element.freq;
    }

    FrequencyOrderLocator<Element<T>> frequency_order_locator;
    ObsFrequencyOrderLocator<Element<T>> obs_frequency_order_locator;
};


}

#endif //_LotterySamplingV2_Types_H_
