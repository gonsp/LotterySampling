#include "Misc.h"

using namespace std;

TicketGenerator::TicketGenerator(bool aging, int seed) {
    this->aging = aging;
    if(seed == -1) {
        seed = random_device()();
    }
    random_state = mt19937_64(seed);
    if(aging) {
        // If aging is used, then the range of the tickets is decreased
        // in half to avoid overflows.
        MAX_TICKET = numeric_limits<uint64_t>::max() >> 1;
    } else {
        MAX_TICKET = numeric_limits<uint64_t>::max();
    }
    dist = uniform_int_distribution<Ticket>(0, MAX_TICKET);
}

Ticket TicketGenerator::generate_ticket(unsigned int N) {
    Ticket ticket = dist(random_state);
    if(aging) {
        ticket += N;
    }
    return ticket;
}

unsigned int TicketGenerator::estimate_frequency(const Ticket& min_ticket) const {
    // TODO Protect from infinity
    // TODO take into account aging
    return static_cast<unsigned int>(1 / (1 - min_ticket / double(MAX_TICKET)));
}

Ticket TicketGenerator::incremental_averaging(const Ticket& old_mean, const Ticket& ticket, unsigned int n) const {
    // To avoid overflows
    if(ticket > old_mean) {
        return old_mean + (ticket - old_mean) / n;
    } else {
        return old_mean - (old_mean - ticket) / n;
    }
}

Ticket TicketGenerator::decremenetal_averaging(const Ticket& old_mean, const Ticket& ticket, unsigned int n) const {
    // To avoid overflows
    if(ticket > old_mean) {
        return old_mean - (ticket - old_mean) / n;
    } else {
        return old_mean + (old_mean - ticket) / n;
    }
}