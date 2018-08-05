#ifndef _Types_LotterySampling_H_
#define _Types_LotterySampling_H_

#include <set>

namespace LotterySampling {

template <class T>
struct Element;

template <class T>
struct ElementLocator {
    typedef std::shared_ptr<Element<T>> type;
};

template <class T>
struct CompareTicket {
    bool operator()(const typename ElementLocator<T>::type& a, const typename ElementLocator<T>::type& b) const;
};

template <class T>
struct CompareFrequency {
    bool operator()(const typename ElementLocator<T>::type& a, const typename ElementLocator<T>::type& b) const;
};

template <class T>
struct TicketOrder {
    typedef std::multiset<typename ElementLocator<T>::type, CompareTicket<T>> type;
};

template <class T>
struct FrequencyOrder {
    typedef std::multiset<typename ElementLocator<T>::type, CompareFrequency<T>> type;
};

typedef uint64_t Ticket;

template <class T>
struct Element {
    T id;
    Ticket ticket;
    unsigned int freq;
    unsigned int over_estimation;

    typename TicketOrder<T>::type::iterator ticket_iterator;
    int level;

    typename FrequencyOrder<T>::type::iterator frequency_iterator;

    Element(const T& id, Ticket ticket, unsigned int freq, unsigned int over_estimation) {
        this->id = id;
        this->ticket = ticket;
        this->freq = freq;
        this->over_estimation = over_estimation;
    }

    bool operator<(const Element<T>& element) const {
        return this->freq < element.freq;
    }
};

template <class T>
bool CompareTicket<T>::operator()(const typename ElementLocator<T>::type& a, const typename ElementLocator<T>::type& b) const {
    return a->ticket < b->ticket;
};

template <class T>
bool CompareFrequency<T>::operator()(const typename ElementLocator<T>::type& a, const typename ElementLocator<T>::type& b) const {
    return *a < *b;
}


}

#endif //_Types_LotterySampling_H_
