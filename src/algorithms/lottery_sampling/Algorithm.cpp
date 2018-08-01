#include "Algorithm.h"
#include <iostream>

using namespace LotterySampling;
using namespace std;

Algorithm::Algorithm(const InputParser& parameters) {
    m = (unsigned  int) stoul(parameters.get_parameter("-m"));
    aging = parameters.has_parameter("-aging");
    multilevel = parameters.has_parameter("-multilevel");
    if(multilevel) {
        set_monitored_size(m);
    }
    int seed;
    if(parameters.has_parameter("-seed")) {
        seed = stoi(parameters.get_parameter("-seed"));
    } else {
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

void Algorithm::frequent_query(float f, std::ostream& stream) {
    for(auto it = frequency_order.rbegin(); it != frequency_order.rend() && (*it)->freq >= f*N; ++it) {
        stream << (*it)->id << " " << (*it)->freq / (float) N << endl;
    }
}

void Algorithm::k_top_query(int k, std::ostream& stream) {
    for(auto it = frequency_order.rbegin(); it != frequency_order.rend() && k-- > 0; ++it) {
        stream << (*it)->id << " " << (*it)->freq / (float) N << endl;
    }
}

void Algorithm::free_up_level_1() {
    ElementLocator replaced_element_locator = *level_1.begin();
    level_1.erase(level_1.begin());
    if(multilevel) {
        // Element is kicked out from level 1 to level 2
        replaced_element_locator->ticket_iterator = level_2.emplace_hint(prev(level_2.end()), replaced_element_locator);
        replaced_element_locator->level = 2;
    } else {
        // Element is just removed since multilevel is not being used
        frequency_order.erase(replaced_element_locator->frequency_iterator);
        remove_element(replaced_element_locator->id);
    }
}

void Algorithm::free_up_level_2() {
    // Element is removed
    ElementLocator locator = *level_2.begin();
    string removed_id = locator->id;
    level_2.erase(level_2.begin());
    frequency_order.erase(locator->frequency_iterator);
    remove_element(removed_id);
}

bool Algorithm::insert_element(std::string& element_id, ElementLocator& locator) {

    Ticket ticket = generate_ticket();
    unsigned int freq;
    int level;

    if(sample_size() < m) {
        freq = 1;
        level = 1;
    } else {
        if(ticket > (*level_1.begin())->ticket) {
            freq = estimate_frequency((*level_1.begin())->ticket);
            level = 1;
            free_up_level_1();
        } else if(!level_2.empty() && ticket > (*level_2.begin())->ticket) {
            freq = estimate_frequency((*level_2.begin())->ticket);
            level = 2;
            free_up_level_2();
        } else {
            // New element didn't get a good enough ticket to get sampled, so it's discarded
            level = -1;
        }
    }

    if(level != -1) {
        locator = make_shared<Element>(element_id, ticket, freq);
        locator->ticket_iterator = (level == 1 ? level_1 : level_2).insert(locator);
        locator->level = level;
        locator->frequency_iterator = frequency_order.insert(locator);
        return true;
    }
    return false;
}

void Algorithm::update_element(ElementLocator& locator) {
    // Updating frequency
    FrequencyOrder::iterator hint = next(locator->frequency_iterator);
    frequency_order.erase(locator->frequency_iterator); // It's needed to remove and reinsert an element since there isn't an "update" method in multiset
    locator->freq++;
    locator->frequency_iterator = frequency_order.emplace_hint(hint, locator);

    // Updating ticket
    Ticket old_ticket = locator->ticket;
    Ticket ticket = generate_ticket();
    if(ticket > old_ticket) { // The new ticket is better than the old one
        Ticket level_1_threshold = (*level_1.begin())->ticket;
        if(locator->level == 2 && level_1_threshold < ticket) {
            // element is moving from level_2 to level_1, so we kick out the lowest ticket from level_1 to level_2
            free_up_level_1();
        }

        TicketOrder::iterator hint = next(locator->ticket_iterator);
        (locator->level == 1 ? level_1 : level_2).erase(locator->ticket_iterator);
        locator->ticket = ticket; // Updating (the better) ticket
        locator->ticket_iterator = (ticket > level_1_threshold ? level_1 : level_2).emplace_hint(hint, locator);
        locator->level = (ticket > level_1_threshold ? 1 : 2);
    }
}

Ticket Algorithm::generate_ticket() {
    Ticket ticket = dist(random_state);
    if(aging) {
        ticket += N;
    }
    return ticket;
}

inline unsigned int Algorithm::estimate_frequency(Ticket min_ticket) const {
    // TODO Protect from infinity
    return static_cast<unsigned int>(1 / (1 - min_ticket / (double) MAX_TICKET));
}

template<typename T>
void print_container(const T& container) {
    for(auto it = container.rbegin(); it != container.rend(); ++it) {
        cout << (*it)->id << ", " << (*it)->ticket << ", " << (*it)->freq << endl;
    }
}

void Algorithm::print_state() {
    cout << "-----------------------" << endl;
    cout << "%%%%%% level_1 %%%%%%" << endl;
    print_container(level_1);
    cout << "-----------------------" << endl;
    cout << "%%%%%% level_2 %%%%%%" << endl;
    print_container(level_2);
    cout << "-----------------------" << endl;
    cout << "%%%%%% frequency_order %%%%%%" << endl;
    print_container(frequency_order);
    assert(level_1.size() + level_2.size() == sample_size());
    assert(frequency_order.size() == sample_size());
}
