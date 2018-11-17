#ifndef _LotterySampling_Types_H_
#define _LotterySampling_Types_H_

#include "data_structures/BinaryHeap.h"
#include "data_structures/SortedList.h"
#include "data_structures/SortedVector.h"
#include "utils/TicketUtils.h"

namespace LotterySampling {


template<class Element>
using TicketOrder = BinaryHeap::BinaryHeap<Element, &Element::compare_ticket, &Element::ticket_order_locator>;

template<class Element>
//using FrequencyOrder = SortedList::SortedList<Element, &Element::frequency_order_locator>;
using FrequencyOrder = SortedVector::SortedVector<Element, &Element::frequency_order_locator>;

template<class Element>
//using FrequencyOrderLocator = SortedList::Locator<Element>;
using FrequencyOrderLocator = SortedVector::Locator;

using TicketOrderLocator = BinaryHeap::Locator;

template<class Element>
//using FrequencyOrderIterator = SortedList::Iterator<Element>;
using FrequencyOrderIterator = SortedVector::Iterator<Element>;

template<class T>
struct Element {

    T id;
    Ticket ticket;
//    unsigned int over_estimation;

    Element(const T& id) {
        this->id = id;
    }

    unsigned int get_count() const {
        return frequency_order_locator.bucket_iterator->key;
    }

    bool compare_ticket(const Element<T>& element) const {
        return this->ticket < element.ticket;
    }

    FrequencyOrderLocator<Element<T>> frequency_order_locator;
    TicketOrderLocator ticket_order_locator = -1;
};


}

#endif //_LotterySampling_Types_H_
