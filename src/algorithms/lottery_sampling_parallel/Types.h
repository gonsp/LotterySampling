#ifndef _LotterySamplingParallel_Types_H_
#define _LotterySamplingParallel_Types_H_

#include "data_structures/BinaryHeap.h"
#include "data_structures/SortedTree.h"
#include "utils/TicketUtils.h"
#include <unordered_map>

namespace LotterySamplingParallel {

    
using namespace std;

template<class Element>
using FrequencyOrder = SortedTree::SortedTree<Element, &Element::compare_freq, &Element::frequency_order_locator>;

template<class Element>
using TicketOrder = BinaryHeap::BinaryHeap<Element, &Element::compare_ticket, &Element::ticket_order_locator>;

template<class Element>
using FrequencyOrderLocator = SortedTree::Locator<Element, &Element::compare_freq>;

using TicketOrderLocator = BinaryHeap::Locator;


template<class T>
struct Element {

    struct ElementInstance {
        Element<T>* element;
        Ticket ticket;

        ElementInstance(Element<T>* element, const Ticket& ticket) {
            this->element = element;
            this->ticket = ticket;
        }

        bool compare_ticket(const ElementInstance& element_instance) const {
            return this->ticket < element_instance.ticket;
        }

        TicketOrderLocator ticket_order_locator = -1;
    };

    T id;
    unsigned int observed_count;
    unordered_map<int, ElementInstance> instances;

    Element(const T& id) {
        this->id = id;
    }

    unsigned int get_count() const {
        return observed_count;
    }

    bool compare_freq(const Element<T>& element) const {
        return this->observed_count < element.observed_count;
    }

    FrequencyOrderLocator<Element<T>> frequency_order_locator;

};

template<class T>
using ElementInstance = typename Element<T>::ElementInstance;


}

#endif //_LotterySamplingParallel_Types_H_
