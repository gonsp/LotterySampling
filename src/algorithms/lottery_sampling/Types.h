#ifndef _LotterySampling_Types_H_
#define _LotterySampling_Types_H_

#include <vector>
#include <data_structures/BinaryHeap.h>
#include "data_structures/SortedList.h"
#include "data_structures/SortedVector.h"
#include "utils/Misc.h"

namespace LotterySampling {


using namespace std;

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
    vector<Ticket> tickets;

    unsigned int over_estimation;

    Element(const T& id) {
        this->id = id;
    }

    unsigned int get_count() const {
        return frequency_order_locator.bucket_iterator->key;
    }

    bool compare_ticket(const Element<T>& element) const {
        return this->ticket < element.ticket;
    }

    bool update_tickets(TicketGenerator& ticket_generator, unsigned int h) {
        if(tickets.size() != h) {
            tickets = vector<Ticket>(h, 0);
        }
        Ticket old_ticket = ticket;
        ticket = 0;
        for(int i = 0; i < h; ++i) {
            tickets[i] = max(tickets[i], ticket_generator.generate_ticket());
            ticket_generator.incremental_averaging(ticket, tickets[i], i + 1);
        }
        return ticket > old_ticket;
    }

    FrequencyOrderLocator<Element<T>> frequency_order_locator;
    TicketOrderLocator ticket_order_locator;
};


}

#endif //_LotterySampling_Types_H_
