#ifndef _Types_LotterySampling_H_
#define _Types_LotterySampling_H_

#include <string>
#include <set>

namespace LotterySampling {


typedef uint64_t Ticket;

struct Element {
    std::string id;
    Ticket ticket;
    int freq;

    Element(std::string id, Ticket ticket, int freq) {
        this->id = id;
        this->ticket = ticket;
        this->freq = freq;
    }

    bool operator< (const Element& element) const {
        return this->ticket < element.ticket;
    }
};

typedef std::multiset<Element> StreamSummary;

struct ElementLocator {
    StreamSummary::iterator element_iterator;
    bool is_level_1;
};


}

#endif //_Types_LotterySampling_H_
