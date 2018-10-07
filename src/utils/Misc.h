#ifndef _Misc_H_
#define _Misc_H_

#include <random>
#include <cstdint>

template<class ClassName>
using ComparatorFunction = bool (ClassName::*)(const ClassName& element) const;

template<class ClassName, class T>
using ClassField = T ClassName::*;

template<class ClassName, ComparatorFunction<ClassName> compare>
struct PointerComparator {
    bool operator()(const ClassName* a, const ClassName* b) const {
        return (a->*compare)(*b);
    }
};


typedef uint64_t Ticket;

class TicketGenerator {

private:

    std::mt19937_64 random_state;
    std::uniform_int_distribution<Ticket> dist;

public:

    Ticket MAX_TICKET;

    TicketGenerator() {}

    TicketGenerator(int seed);

    Ticket generate_ticket(unsigned int N);

    float normalize_ticket(Ticket ticket, unsigned int N) const;

    unsigned int estimate_frequency(const Ticket& min_ticket) const;

    void incremental_averaging(Ticket& old_mean, const Ticket& ticket, unsigned int n) const;

    void decremental_averaging(Ticket& old_mean, const Ticket& ticket, unsigned int n) const;
};


#endif //_Misc_H_
