#ifndef _BasicLotterySamplingTopK_Types_H_
#define _BasicLotterySamplingTopK_Types_H_

#include "data_structures/BinaryHeap.h"
#include "data_structures/SortedList.h"
#include "utils/TicketUtils.h"


namespace BasicLotterySamplingTopK {


template<class Element>
using FrequencyOrder = SortedList::SortedList<Element, &Element::frequency_order_locator>;

template<class Element>
using TicketOrder = BinaryHeap::BinaryHeap<Element, &Element::compare_ticket, &Element::ticket_order_locator>;

template<class Element>
using FrequencyOrderLocator = SortedList::Locator<Element>;

using TicketOrderLocator = BinaryHeap::Locator;


template<class T>
struct Element {

    T id;
    unsigned int freq;
    Ticket ticket;

    Element(const T& id) {
        this->id = id;
    }

    unsigned int get_freq() const {
        return freq;
    }

    bool compare_ticket(const Element<T>& element) const {
        return this->ticket < element.ticket;
    }

    TicketOrderLocator ticket_order_locator = -1;
    FrequencyOrderLocator<Element<T>> frequency_order_locator;
};


}

#endif //_BasicLotterySamplingTopK_Types_H_
