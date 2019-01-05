#include "TicketUtils.h"

using namespace std;

TicketUtils::TicketUtils(int seed) {
    if(seed == -1) {
        seed = random_device()();
    }
    random_state = mt19937_64(seed);
    dist = uniform_int_distribution<Ticket>(0, MAX_TICKET);
}

Ticket TicketUtils::generate_ticket() {
    Ticket ticket = dist(random_state);
    return ticket;
}

double TicketUtils::normalize_ticket(const Ticket& ticket) {
    return ticket / double(MAX_TICKET);
}

Ticket TicketUtils::estimate_ticket(unsigned int count) {
    return Ticket(count / double(count + 1) * MAX_TICKET);
}

unsigned int TicketUtils::estimate_count(const Ticket& ticket) {
    double normalized_ticket = TicketUtils::normalize_ticket(ticket);
    unsigned int estimated_count = normalized_ticket / (1 - normalized_ticket);
    return estimated_count;
}

unsigned int TicketUtils::estimate_count_geometric(const Ticket& min_ticket) {
    // TODO Protect from infinity
    double normalized_min_ticket = TicketUtils::normalize_ticket(min_ticket);
    unsigned int estimated_count = 1 / (1 - normalized_min_ticket);
    return estimated_count;
}

// n is the number of elements TO BE aggregated in mean (after calling the function)
void TicketUtils::incremental_averaging(Ticket& mean, const Ticket& ticket, unsigned int n) {
    // To avoid overflows
    if(ticket > mean) {
        mean = mean + (ticket - mean) / n;
    } else {
        mean = mean - (mean - ticket) / n;
    }
}

// n is the number of elements aggregated in mean (before calling the function)
void TicketUtils::decremental_averaging(Ticket& mean, const Ticket& ticket, unsigned int n) {
    if(n == 1) {
        mean = 0;
    } else {
        mean = ((mean - ticket / n) / (n - 1)) * n;
    }
}