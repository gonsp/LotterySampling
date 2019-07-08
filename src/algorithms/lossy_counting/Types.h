#ifndef _LossyCounting_Types_H_
#define _LossyCounting_Types_H_

#include "data_structures/SortedTree.h"


namespace LossyCounting {


template<class Element>
using FrequencyOrder = SortedTree::SortedTree<Element, &Element::compare_freq, &Element::frequency_order_locator>;

template<class Element>
using FrequencyOrderLocator = SortedTree::Locator<Element, &Element::compare_freq>;


template<class T>
struct Element {

    T id;
    unsigned int freq;

    Element(const T& id) {
        this->id = id;
    }

    bool compare_freq(const Element<T>& element) const {
        return this->freq < element.freq;
    }

    unsigned int get_freq() const {
        return freq;
    }

    FrequencyOrderLocator<Element<T>> frequency_order_locator;
};


}


#endif //_LossyCounting_Types_H_
