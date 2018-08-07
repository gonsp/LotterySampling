#ifndef _Types_LotterySampling_H_
#define _Types_LotterySampling_H_

#include <set>

namespace LotterySampling {

    
using namespace std;

typedef uint64_t Ticket;

template <class T>
struct Element {

private:
    struct CompareTicket {
        inline bool operator()(const Element<T>* a, const Element<T>* b) const {
            return a->ticket < b->ticket;
        }
    };

    struct CompareFrequency {
        inline bool operator()(const Element<T>* a, const Element<T>* b) const {
            return *a < *b;
        }
    };

public:
    typedef multiset<Element<T>*, CompareTicket> TicketOrder;
    typedef multiset<Element<T>*, CompareFrequency> FrequencyOrder;

    T id;
    Ticket ticket;
    unsigned int freq;
    unsigned int over_estimation;

    typename TicketOrder::iterator ticket_iterator;
    int level;

    typename FrequencyOrder::iterator frequency_iterator;

    Element(const T& id) {
        this->id = id;
    }

    bool operator<(const Element<T>& element) const {
        return this->freq < element.freq;
    }
};


}

#endif //_Types_LotterySampling_H_
