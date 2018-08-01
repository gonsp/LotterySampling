#include "Types.h"

using namespace LotterySampling;

Element::Element(std::string id, Ticket ticket, unsigned int freq) {
    this->id = id;
    this->ticket = ticket;
    this->freq = freq;
}

bool Element::operator<(const Element& element) const {
    return this->freq < element.freq;
}

bool CompareTicket::operator()(const ElementLocator& a, const ElementLocator& b) const {
    return a->ticket < b->ticket;
};

bool CompareFrequency::operator()(const ElementLocator& a, const ElementLocator& b) const {
    return *a < *b;
}