#ifndef _LossyCounting_Types_H_
#define _LossyCounting_Types_H_

#include "data_structures/SortedVector.h"

namespace LossyCounting {


template<class Element>
using FrequencyOrder = SortedList::SortedList<Element, &Element::frequency_order_locator>;

template<class Element>
using EstimatedFrequencyOrder = SortedTree::SortedTree<Element, &Element::compare_freq, &Element::estimated_frequency_order_locator>;

template<class Element>
using FrequencyOrderLocator = SortedList::Locator<Element>;

template<class Element>
using EstimatedFrequencyOrderLocator = SortedTree::Locator<Element, &Element::compare_freq>;


template<class T>
struct Element {

    T id;
    unsigned int freq;
    unsigned int over_estimation;

    Element(const T& id) {
        this->id = id;
    }

    bool compare_freq(const Element<T>& element) const {
        return this->freq + this->over_estimation < element.freq + element.over_estimation;
    }

    unsigned int get_freq() const {
        return freq;
    }

    FrequencyOrderLocator<Element<T>> frequency_order_locator;
    EstimatedFrequencyOrderLocator<Element<T>> estimated_frequency_order_locator;
};


}


#endif //_LossyCounting_Types_H_
