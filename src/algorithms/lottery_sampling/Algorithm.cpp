#include "Algorithm.h"
#include <iostream>

using namespace LotterySampling;
using namespace std;

Algorithm::Algorithm(const InputParser& parameters) {
    m = (unsigned  int) stoul(parameters.get_parameter("-m"));
    aging = parameters.has_parameter("-aging");
    if(parameters.has_parameter("-seed")) {
        seed = stoi(parameters.get_parameter("-seed"));
    } else {
        seed = random_device()();
    }
    rng = mt19937_64(seed);
    if(aging) {
        // If aging is used, then the range of the tickets is decreased
        // in half to avoid overflows.
        MAX_TICKET = static_cast<unsigned long long int>(numeric_limits<int64_t>::max() - 1);
        dist = uniform_int_distribution<Ticket>(0, MAX_TICKET);
    } else {
        MAX_TICKET = numeric_limits<uint64_t>::max();
    }
}

void Algorithm::frequent_query(float f, std::ostream& stream) {

}

void Algorithm::k_top_query(int k, std::ostream& stream) {

}

void Algorithm::free_up_level_1() {
    // Element is kicked out from level 1 to level 2
    Element replaced_element = *level_1.begin();
    level_1.erase(level_1.begin());
    ElementLocator& replaced_locator = get_locator(replaced_element.id);
    replaced_locator.element_iterator = level_2.insert(replaced_element);
    replaced_locator.level = 2;
}

void Algorithm::free_up_level_2() {
    // Element is removed
    string removed_id = level_2.begin()->id;
    level_2.erase(level_2.begin());
    remove_element(removed_id);
}

ElementLocator Algorithm::insert_element(std::string& element_id) {
    ElementLocator locator;
    Ticket ticket = generate_ticket();
    if(sample_size() < m) {
        locator.element_iterator = level_1.emplace(element_id, ticket, 1);
        locator.level = 1;
    } else {
        unsigned int freq;
        if(ticket > level_1.begin()->ticket) {
            free_up_level_1();

            freq = estimate_frequency(level_1.begin()->ticket);
            locator.element_iterator = level_1.emplace(element_id, ticket, freq);
            locator.level = 1;
        } else if(!level_2.empty() && ticket > level_2.begin()->ticket) {
            free_up_level_2();

            freq = estimate_frequency(level_2.begin()->ticket);
            locator.element_iterator = level_2.emplace(element_id, ticket, freq);
            locator.level = 2;
        } else {
            // New element didn't get a good enough ticket to get sampled, so it's discarded
            locator.level = -1;
        }
    }
    return locator;
}

void Algorithm::update_element(ElementLocator& locator) {
    const_cast<Element&>(*locator.element_iterator).freq++; // Casting needed because multiset::iterator is const
    Ticket old_ticket = locator.element_iterator->ticket;
    Ticket ticket = generate_ticket();
    if(ticket > old_ticket) { // The new ticket is better than the old one
        Ticket level_1_threshold = level_1.begin()->ticket;
        if(locator.level == 2 && level_1_threshold < ticket) {
            // element is moving from level_2 to level_1, so we kick out the lowest ticket from level_1 to level_2
            free_up_level_1();
        }

        Element element = *locator.element_iterator;
        element.ticket = ticket; // Updating (the better) ticket
        (locator.level == 1 ? level_1 : level_2).erase(locator.element_iterator);
        locator.element_iterator = (ticket > level_1_threshold ? level_1 : level_2).insert(element);
        locator.level = (ticket > level_1_threshold ? 1 : 2);
    }
}

Ticket Algorithm::generate_ticket() {
    Ticket ticket = dist(rng);
    if(aging) {
        ticket += N;
    }
    return ticket;
}

inline unsigned int Algorithm::estimate_frequency(Ticket min_ticket) const {
    // TODO Protect from infinity
    return static_cast<unsigned int>(1 / (1 - min_ticket / (double) MAX_TICKET));
}

void print_level(const StreamSummary& level) {
    for(auto it = level.rbegin(); it != level.rend(); ++it) {
        cout << it->id << ", " << it->ticket << ", " << it->freq << endl;
    }
}

void Algorithm::print_state() {
    cout << "-----------------------" << endl;
    cout << "%%%%%% level_1 %%%%%%" << endl;
    print_level(level_1);
    cout << "-----------------------" << endl;
    cout << "%%%%%% level_2 %%%%%%" << endl;
    print_level(level_2);
}
