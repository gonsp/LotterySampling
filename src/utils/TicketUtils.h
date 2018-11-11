#ifndef _TicketUtils_H_
#define _TicketUtils_H_

#include <random>

typedef uint64_t Ticket;

class TicketUtils {

private:

    std::mt19937_64 random_state;
    std::uniform_int_distribution<Ticket> dist;

public:

    static const Ticket MAX_TICKET = std::numeric_limits<Ticket>::max();

    TicketUtils() {}

    TicketUtils(int seed);

    Ticket generate_ticket();

    static double normalize_ticket(const Ticket& ticket);

    static Ticket estimate_ticket(unsigned int count);

    static unsigned int estimate_count(const Ticket& ticket);

    static unsigned int estimate_count_geometric(const Ticket& min_ticket);

    static void incremental_averaging(Ticket& old_mean, const Ticket& ticket, unsigned int n);

    static void decremental_averaging(Ticket& old_mean, const Ticket& ticket, unsigned int n);
};

#include <random>

#endif //_TicketUtils_H_
