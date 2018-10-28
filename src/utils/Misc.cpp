#include "Misc.h"

using namespace std;

TicketGenerator::TicketGenerator(int seed) {
    if(seed == -1) {
        seed = random_device()();
    }
    random_state = mt19937_64(seed);
    dist = uniform_int_distribution<Ticket>(0, MAX_TICKET);
}

Ticket TicketGenerator::generate_ticket() {
    Ticket ticket = dist(random_state);
    return ticket;
}

double TicketGenerator::normalize_ticket(Ticket ticket) {
    return ticket / double(MAX_TICKET);
}

unsigned int TicketGenerator::estimate_count(const Ticket& ticket) {
    double normalized_ticket = TicketGenerator::estimate_count(ticket);
    unsigned int estimated_count = normalized_ticket / (1 - normalized_ticket);
    return estimated_count;
}

unsigned int TicketGenerator::estimate_count_geometric(const Ticket& min_ticket) {
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