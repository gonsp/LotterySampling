#ifndef _Types_LotterySampling_H_
#define _Types_LotterySampling_H_

#include <string>
#include <set>

namespace LotterySampling {


struct Element;

typedef std::shared_ptr<Element> ElementLocator;

struct CompareTicket {
    bool operator()(const ElementLocator& a, const ElementLocator& b) const;
};

struct CompareFrequency {
    bool operator()(const ElementLocator& a, const ElementLocator& b) const;
};

typedef std::multiset<ElementLocator, CompareTicket> TicketOrder;

typedef std::multiset<ElementLocator, CompareFrequency> FrequencyOrder;

typedef uint64_t Ticket;

struct Element {
    std::string id;
    Ticket ticket;
    unsigned int freq;
    unsigned int over_estimation;

    TicketOrder::iterator ticket_iterator;
    int level;

    FrequencyOrder::iterator frequency_iterator;

    Element(std::string id, Ticket ticket, unsigned int freq, unsigned int over_estimation);

    bool operator<(const Element& element) const;
};


}

#endif //_Types_LotterySampling_H_
