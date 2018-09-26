#include "Misc.h"

using namespace std;

TicketGenerator::TicketGenerator(unsigned int window_size, int seed) {
    this->window_size = window_size;
    if(seed == -1) {
        seed = random_device()();
    }
    random_state = mt19937_64(seed);
    if(window_size != 0) {
        MAX_TICKET = numeric_limits<uint32_t>::max();
    } else {
        MAX_TICKET = numeric_limits<uint64_t>::max();
    }
    dist = uniform_int_distribution<Ticket>(0, MAX_TICKET);
}

Ticket TicketGenerator::generate_ticket(unsigned int N) {
    Ticket ticket = dist(random_state);
    if(window_size != 0) {
        ticket += (MAX_TICKET / window_size) * N;
    }
    return ticket;
}

float TicketGenerator::normalize_ticket(Ticket ticket, unsigned int N) const {
    if(window_size != 0) {
        ticket -= (MAX_TICKET / window_size) * N;
    }
    return ticket / float(MAX_TICKET);
}

unsigned int TicketGenerator::estimate_frequency(const Ticket& min_ticket) const {
    // TODO Protect from infinity
    // TODO take into account aging
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