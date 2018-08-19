#ifndef _Element_SpaceSaving_H_
#define _Element_SpaceSaving_H_

#include <data_structures/SortedList.h>

namespace SpaceSaving {


template<class Element>
using FrequencyOrder = SortedList::SortedList<Element, &Element::frequency_order_locator>;

template<class Element>
using FrequencyOrderLocator = SortedList::Locator<Element>;

template<class T>
struct Element {

public:

    T id;
    unsigned int over_estimation;

    FrequencyOrderLocator<Element<T>> frequency_order_locator;

    Element(const T& id) {
        this->id = id;
    }
};


}

#endif //_Element_SpaceSaving_H_
