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

    static const Ticket MAX_TICKET = std::numeric_limits<uint64_t>::max();

    TicketGenerator() {}

    TicketGenerator(int seed);

    Ticket generate_ticket();

    static double normalize_ticket(Ticket ticket);

    static unsigned int estimate_count(const Ticket& ticket);

    static unsigned int estimate_count_geometric(const Ticket& min_ticket);

    void incremental_averaging(Ticket& old_mean, const Ticket& ticket, unsigned int n) const;

    void decremental_averaging(Ticket& old_mean, const Ticket& ticket, unsigned int n) const;
};


#endif //_Misc_H_
