#ifndef _Types_LotterySampling_H_
#define _Types_LotterySampling_H_

#include <string>
#include <set>

namespace LotterySampling {


struct Element {
    std::string id;
    uint64_t ticket;
    int freq;

    Element(std::string id, uint64_t ticket, int freq) {
        this->id = id;
        this->ticket = ticket;
        this->freq = freq;
    }

    bool operator< (const Element& element) const {
        return this->ticket < element.ticket;
    }
};

typedef std::multiset<Element> StreamSummary;


}

#endif //_Types_LotterySampling_H_
