#ifndef _BasicLotterySampling_Types_H_
#define _BasicLotterySampling_Types_H_

#include "data_structures/BinaryHeap.h"
#include "data_structures/SortedTree.h"
#include "utils/TicketUtils.h"

namespace BasicLotterySampling {


template<class Element>
using FrequencyOrder = SortedTree::SortedTree<Element, &Element::compare_freq, &Element::frequency_order_locator>;

template<class Element>
using TicketOrder = BinaryHeap::BinaryHeap<Element, &Element::compare_ticket, &Element::ticket_order_locator>;

template<class Element>
using FrequencyOrderLocator = SortedTree::Locator<Element, &Element::compare_freq>;

using TicketOrderLocator = BinaryHeap::Locator;


template<class T>
struct Element {

    T id;
    Ticket ticket;
    unsigned int freq;

    Element(const T& id) {
        this->id = id;
    }

    bool compare_freq(const Element<T>& element) const {
        return this->freq < element.freq;
    }

    bool compare_ticket(const Element<T>& element) const {
        return this->ticket < element.ticket;
    }

    unsigned int get_freq() const {
        return freq;
    }

    TicketOrderLocator ticket_order_locator = -1;
    int level;

    FrequencyOrderLocator<Element<T>> frequency_order_locator;
};


}

#endif //_BasicLotterySampling_Types_H_
