#ifndef _Element_LotterySampling_H_
#define _Element_LotterySampling_H_

#include "data_structures/TicketOrder.h"
#include "data_structures/FrequencyOrder.h"

namespace LotterySampling {

    
using namespace std;

typedef uint64_t Ticket;

template<class T>
struct Element {

public:

    T id;
    Ticket ticket;
    unsigned int freq;
    unsigned int over_estimation;

    typename TicketOrder<Element<T>>::iterator ticket_order_iterator;
    int level;

    typename FrequencyOrder<Element<T>>::iterator frequency_order_iterator;

    Element(const T& id) {
        this->id = id;
    }

    bool operator<(const Element<T>& element) const {
        if(this->freq != element.freq) {
            return this->freq < element.freq;
        } else {
            return this->over_estimation > element.over_estimation;
        }
    }
};


}

#endif //_Element_LotterySampling_H_
