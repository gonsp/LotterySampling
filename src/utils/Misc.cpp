#include "Misc.h"

using namespace std;

TicketGenerator::TicketGenerator(int seed) {
    if(seed == -1) {
        seed = random_device()();
    }
    random_state = mt19937_64(seed);
    MAX_TICKET = numeric_limits<uint64_t>::max();
    dist = uniform_int_distribution<Ticket>(0, MAX_TICKET);
}

Ticket TicketGenerator::generate_ticket(unsigned int N) {
    Ticket ticket = dist(random_state);
    return ticket;
}

float TicketGenerator::normalize_ticket(Ticket ticket, unsigned int N) const {
    return ticket / float(MAX_TICKET);
}

unsigned int TicketGenerator::estimate_frequency(const Ticket& min_ticket) const {
    // TODO Protect from infinity
    return static_cast<unsigned int>(1 / (1 - min_ticket / double(MAX_TICKET)));
}

void TicketGenerator::incremental_averaging(Ticket& mean, const Ticket& ticket, unsigned int n) const {
    // To avoid overflows
    if(ticket > mean) {
        mean = mean + (ticket - mean) / n;
    } else {
        mean = mean - (mean - ticket) / n;
    }
}

void TicketGenerator::decremental_averaging(Ticket& mean, const Ticket& ticket, unsigned int n) const {
    if(n == 1) {
        mean = 0;
    } else {
        mean = ((mean - ticket / n) / (n - 1)) * n;
    }
}