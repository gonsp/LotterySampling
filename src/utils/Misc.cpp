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
        MAX_TICKET = static_cast<unsigned long long int>(numeric_limits<int64_t>::max() - 1);
        dist = uniform_int_distribution<Ticket>(0, MAX_TICKET);
    } else {
        MAX_TICKET = numeric_limits<uint64_t>::max();
    }
}

Ticket TicketGenerator::generate_ticket(unsigned int N) {
    Ticket ticket = dist(random_state);
    if(aging) {
        ticket += N;
    }
    return ticket;
}

unsigned int TicketGenerator::estimate_frequency(Ticket min_ticket) const {
    // TODO Protect from infinity
    // TODO take into account aging
    return static_cast<unsigned int>(1 / (1 - min_ticket / (double) MAX_TICKET));
}