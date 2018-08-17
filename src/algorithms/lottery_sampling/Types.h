#ifndef _Element_LotterySampling_H_
#define _Element_LotterySampling_H_

#include "data_structures/BinaryHeap.h"
#include "data_structures/SortedTree.h"

namespace LotterySampling {

    
using namespace std;

template<class Element>
using FrequencyOrder = SortedTree::SortedTree<Element, &Element::compare_freq, &Element::frequency_order_iterator>;

template<class Element>
using TicketOrder = BinaryHeap::BinaryHeap<Element, &Element::compare_ticket, &Element::ticket_order_iterator>;

template<class Element>
using FrequencyOrderIterator = SortedTree::Iterator<Element, &Element::compare_freq>;

using TicketOrderIterator = BinaryHeap::Iterator;

typedef uint64_t Ticket;

template<class T>
struct Element {

public:

    T id;
    Ticket ticket;
    unsigned int freq;
    unsigned int over_estimation;

    Element(const T& id) {
        this->id = id;
    }

    bool compare_freq(const Element<T>& element) const {
        if(this->freq != element.freq) {
            return this->freq < element.freq;
        } else {
            return this->over_estimation > element.over_estimation;
        }
    }

    bool compare_ticket(const Element<T>& element) const {
        return this->ticket < element.ticket;
    }

    TicketOrderIterator ticket_order_iterator;
    int level;

    FrequencyOrderIterator<Element<T>> frequency_order_iterator;
};


}

#endif //_Element_LotterySampling_H_
