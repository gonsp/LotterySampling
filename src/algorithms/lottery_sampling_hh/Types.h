#ifndef _LotterySamplingHH_Types_H_
#define _LotterySamplingHH_Types_H_

#include "data_structures/BinaryHeap.h"
#include "data_structures/SortedVector.h"
#include "utils/TicketUtils.h"


namespace LotterySamplingHH {


template<class Element>
using TicketOrder = BinaryHeap::BinaryHeap<Element, &Element::compare_ticket, &Element::ticket_order_locator>;

template<class Element>
using FrequencyOrder = SortedVector::SortedVector<Element, &Element::frequency_order_locator>;

template<class Element>
using FrequencyOrderLocator = SortedVector::Locator;

using TicketOrderLocator = BinaryHeap::Locator;


template<class T>
struct Element {

    T id;
    Ticket ticket;

    Element(const T& id) {
        this->id = id;
    }

    unsigned int get_freq() const {
        return frequency_order_locator.bucket_iterator->key;
    }

    bool compare_ticket(const Element<T>& element) const {
        return this->ticket < element.ticket;
    }

    FrequencyOrderLocator<Element<T>> frequency_order_locator;
    TicketOrderLocator ticket_order_locator = -1;
};


}

#endif //_LotterySamplingHH_Types_H_
