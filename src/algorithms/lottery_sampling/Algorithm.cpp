#include "Algorithm.h"

using namespace LotterySampling;
using namespace std;

Algorithm::Algorithm(const InputParser& parameters) {
    m = (unsigned  int) stoul(parameters.get_parameter("-m"));
    aging = parameters.has_parameter("-aging");
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
    if(size() < m) {
        locator.element_iterator = level_1.emplace(element_id, ticket, 1);
        locator.level = 1;
    } else {
        if(ticket > level_1.begin()->ticket) {
            free_up_level_1();

            locator.element_iterator = level_1.emplace(element_id, ticket, 1);
            locator.level = 1;
        } else if(ticket > level_2.begin()->ticket) {
            free_up_level_2()

            locator.element_iterator = level_2.emplace(element_id, ticket, 1);
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
        if(old_ticket < level_1_threshold && level_1_threshold < ticket) {
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

Ticket Algorithm::generate_ticket() const {
    Ticket ticket = 0; // TODO implement this
    if(aging) {
        ticket += N;
    }
    return ticket;
}

void Algorithm::print_state() {
}
