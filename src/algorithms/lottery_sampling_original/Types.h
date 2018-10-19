#ifndef _LotterySamplingOriginal_Types_H_
#define _LotterySamplingOriginal_Types_H_

#include "data_structures/BinaryHeap.h"
#include "data_structures/SortedTree.h"

namespace LotterySamplingOriginal {

    
using namespace std;

template<class Element>
using FrequencyOrder = SortedTree::SortedTree<Element, &Element::compare_freq, &Element::frequency_order_locator>;

template<class Element>
using TicketOrder = BinaryHeap::BinaryHeap<Element, &Element::compare_ticket, &Element::ticket_order_locator>;

template<class Element>
using FrequencyOrderLocator = SortedTree::Locator<Element, &Element::compare_freq>;

using TicketOrderLocator = BinaryHeap::Locator;

template<class Element>
using FrequencyOrderIterator = SortedTree::Iterator<Element, &Element::compare_freq>;

template<class T>
struct Element {

    T id;
    Ticket mean_ticket;
    vector<Ticket> tickets;

    unsigned int count;
    unsigned int over_estimation;

    Element(const T& id) {
        this->id = id;
    }

    bool compare_freq(const Element<T>& element) const {
        if(this->count != element.count) {
            return this->count < element.count;
        } else {
            return this->over_estimation > element.over_estimation;
        }
    }

    bool compare_ticket(const Element<T>& element) const {
        return this->mean_ticket < element.mean_ticket;
    }

    bool update_tickets(TicketGenerator& ticket_generator, unsigned int h) {
        if(tickets.size() != h) {
            tickets = vector<Ticket>(h, 0);
        }
        Ticket old_mean_ticket = mean_ticket;
        mean_ticket = 0;
        for(int i = 0; i < h; ++i) {
            tickets[i] = max(tickets[i], ticket_generator.generate_ticket());
            ticket_generator.incremental_averaging(mean_ticket, tickets[i], i + 1);
        }
        return mean_ticket > old_mean_ticket;
    }

    TicketOrderLocator ticket_order_locator;
    int level;

    FrequencyOrderLocator<Element<T>> frequency_order_locator;

    unsigned int get_count() const {
        return count;
    }
};


}

#endif //_LotterySamplingOriginal_Types_H_
